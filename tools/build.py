#!/usr/bin/env python3

import os
import yaml
import argparse
import subprocess

from targetdescription import Board

def create_memory_header(board: Board) -> None:
    memories = board.get_memory_devices()
    lines = []

    # TODO(pkoscik): add asserts to verify if there are no duplicated memory types
    # TODO(pkoscik): what about stack :(?
    for name, description in memories.items():
        match description.type:
            case "memory":
                lines.append(f"#define FLASH_START {description.address}")
                lines.append(f"#define FLASH_SIZE {description.size}")
            case "ram":
                lines.append(f"#define RAM_START {description.address}")
                lines.append(f"#define RAM_SIZE {description.size}")

    with open("output/mem_def.h", "w") as f:
        f.write("\n".join(lines))


def create_cflags(board: Board) -> str:
    cflags = [
        "-nostartfiles",
        "-Wall",
        "-O2",
    ]

    cpu = board.get_first_cpu()
    match cpu.arch:
        case "riscv":
            cflags.append(f"-march={cpu.isa}")
            cflags.append(f"-mabi={cpu.abi}")

    return " ".join(cflags)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Build script for the project")
    parser.add_argument("board", nargs='?', help="The name of the board to use")
    parser.add_argument("sample", nargs='?', help="The name of the sample to build")
    parser.add_argument("-c", "--clean", action="store_true", help="Run 'make clean' instead of 'make'")
    parser.add_argument("-r", "--run", action="store_true", help="Run the Renode simulation")

    args = parser.parse_args()

    BOARD = args.board
    SAMPLE = args.sample
    CLEAN = args.clean
    RUN = args.run

    # Verify if we are in the root of the project
    if not os.path.isfile("Makefile"):
        print("[!] Please run this tool from the root directory of the project")
        exit(1)

    # Create output directory
    if not os.path.isdir("output"):
        os.mkdir("output")

    if CLEAN:
        # Run 'make clean' command
        subprocess.run("make clean", shell=True)
    elif RUN and BOARD:
        # Run the Renode simulation
        command = f"./tools/renode/renode --console boards/{BOARD}/renode/run.resc"
        subprocess.run(command, shell=True)
    elif BOARD and SAMPLE:
        # extract data from board.yml file
        with open(f"boards/{BOARD}/info.yaml") as f:
            board = Board.from_yaml(f)

        create_memory_header(board)

        variables = {
            # TODO(pkoscik):
            # * do not assume this path
            # * automatic arch selection
            "GCCDIR": "/home/pkoscik/.local/opt/zephyr-sdk-0.16.8/riscv64-zephyr-elf/bin",
            "SAMPLE": SAMPLE,
            "ARCH": board.get_first_cpu().arch,
            "CFLAGS": create_cflags(board),
        }

        env = os.environ.copy()
        for k, v in variables.items():
            env[k] = v

        subprocess.Popen("make", env=env, shell=True)
    else:
        print("[!] Please provide both board and sample arguments for building or use -c for cleaning or -r for running.")
        exit(1)
