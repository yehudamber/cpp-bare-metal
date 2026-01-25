#include <stdlib.h>
#include <string.h>

void _initBSS(void)
{
    // defined in link.ld
    extern unsigned char __bss_start;
    extern unsigned char _end;

    memset(&__bss_start, 0, &_end - &__bss_start);
}

void _initExceptionHandling(void)
{
    extern void __register_frame(void*); // defined by libgcc

    extern unsigned char __eh_frame_start; // defined in link.ld

    __register_frame(&__eh_frame_start);
}

void _init(void)
{
    // defined by newlib
    extern void __libc_init_array(void);
    extern void __libc_fini_array(void);

    atexit(__libc_fini_array);
    __libc_init_array();
}
