    .extern _stack_top # defined in link.ld

    # defined in init.c
    .extern _initBSS
    .extern _initExceptionHandling
    .extern _init

    .extern _runtimeMain # defined in runtime-main.cpp

    .equ MSTATUS_FS_INITIAL, 0x00004000

    .section .text
    .global _start

_start:
    la sp, _stack_top

    # Initialize BSS region (though we don't really need to on QEMU)
    jal _initBSS

    # Enable FPU
    csrr t0, mstatus
    li   t1, MSTATUS_FS_INITIAL
    or   t0, t0, t1
    csrw mstatus, t0

    jal _initExceptionHandling
    jal _init

    jal _runtimeMain
    j . # _runtimeMain shouldn't return, but just in case
