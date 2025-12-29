#include <cerrno>
#include <cstddef>
#include <cstdint>
#include <utility>

#undef errno
extern int errno;

extern "C" void _exit(int)
{
    static const auto DEVICE = reinterpret_cast<volatile std::uint32_t*>(0x100000);
    static constexpr auto MAGICAL_VALUE = static_cast<std::uint32_t>(0x5555);
    *DEVICE = MAGICAL_VALUE;
    while (true)
        ;
}

extern "C" void* _sbrk(std::ptrdiff_t incr)
{
    // These symbols are defined in link.ld
    extern std::byte _end;
    extern std::byte _stack_bottom;

    static const auto stackBottom = &_stack_bottom;
    static auto heapEnd = &_end;

    if (heapEnd + incr > stackBottom)
    {
        errno = ENOMEM;
        return reinterpret_cast<void*>(-1);
    }

    return std::exchange(heapEnd, heapEnd + incr);
}
