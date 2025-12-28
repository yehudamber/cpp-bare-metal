#include <cstddef>
#include <new>

extern "C"
{
    // provided by newlib, but not by the C++ standard library headers in free-standing mode
    void* malloc(std::size_t);
    void free(void*);
}

void* operator new(std::size_t size)
{
    if (auto ptr = malloc(size))
    {
        return ptr;
    }
    throw std::bad_alloc();
}

void operator delete(void* ptr) noexcept { free(ptr); }

void operator delete(void* ptr, std::size_t) noexcept { free(ptr); }
