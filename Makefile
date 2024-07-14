# The following variables need to be defined:
# * GCCDIR: path to the Zephyr RISC-V GCC
# * CFLAGS: gcc flags
# * ARCH: target architecture
# * SAMPLE: sample to build

# TODO(pkoscik): use plain rv gcc - drop zephyr
CC = $(GCCDIR)/riscv64-zephyr-elf-gcc
OBJDUMP = $(GCCDIR)/riscv64-zephyr-elf-objdump

ARCH_DIR = arch/$(ARCH)
SAMPLE_DIR = samples/$(SAMPLE)

DRIVERS_DIR = drivers
KERNEL_DIR = kernel

# TODO(pkoscik):
# * use wildcards (for arch and kernel directories)
# * generate this list in the build.py (for drivers - selective based on drivers from yaml)
SRC = $(ARCH_DIR)/reset.S \
      $(ARCH_DIR)/idle.c \
      $(KERNEL_DIR)/init.c \
      $(KERNEL_DIR)/sleep.c \
      $(DRIVERS_DIR)/serial/uart_sifive.c \
      $(SAMPLE_DIR)/src/main.c

OUT = output
ELF = $(OUT)/pkos.elf
DMP = $(OUT)/pkos.dmp
LDS = $(OUT)/pkos.ld

.PHONY: all clean

all: $(ELF) $(DMP)

$(LDS):
	$(CC) -E -P -x c -I$(OUT) $(ARCH_DIR)/linker.ld -o $@

$(ELF): $(SRC) $(LDS)
	$(CC) -Iinclude $(CFLAGS) $(SRC) -T $(LDS) $(LDFLAGS) -o $@

$(DMP): $(ELF)
	$(OBJDUMP) -d $(ELF) > $(DMP)

clean:
	rm -f $(OUT)/*
