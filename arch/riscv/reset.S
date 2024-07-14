/* TODO(pkoscik): Multiple CPU support */
#define BOOT_HART 0

.section .text
.globl __reset

__reset:
        call __init

__init:
        csrr a0, mhartid
        li t0, BOOT_HART
        beq a0, t0, __start_primary_core
        j __loop_secondary_cores

__loop_secondary_cores:
        wfi
        j __loop_secondary_cores

__start_primary_core:
        /* TODO(pkoscik): make the SP init generic (LD and this file) */
        li    sp, 0x80002000
        call kernel_exec

kernel_exec:
        call c_kernel_init
