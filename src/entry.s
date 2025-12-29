    # symbols defined by link.ld
    .extern _stack_top
    .extern __bss_start
    .extern _end

    .extern initExceptionHandling # defined in exception.cpp
    .extern callMain              # defined in call-main.cpp

    .equ MSTATUS_FS_INITIAL, 0x00004000

    .section .text
    .global _start

_start:
    la sp, _stack_top

    # Initialize BSS region (though we don't really need to on QEMU)
    la t0, __bss_start
    la t1, _end
init_bss_loop:
    bgeu t0, t1, init_bss_done
    sb zero, 0(t0)
    addi t0, t0, 1
    j init_bss_loop
init_bss_done:

    # Enable FPU
    csrr t0, mstatus
    li   t1, MSTATUS_FS_INITIAL
    or   t0, t0, t1
    csrw mstatus, t0

    jal initExceptionHandling

    jal callMain
    j . # callMain shouldn't return, but just in case
