#include <cstddef>

extern "C" void __register_frame(void*); // provided by libgcc

extern std::byte __eh_frame_start; // defined in link.ld

extern "C" void initExceptionHandling() { __register_frame(&__eh_frame_start); }
