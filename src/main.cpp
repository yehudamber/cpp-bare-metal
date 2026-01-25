#include "main.hpp"
#include "uart.hpp"

#include <memory>

std::unique_ptr<std::byte[]> tryAllocate(std::size_t size)
{
    try
    {
        auto ptr = std::unique_ptr<std::byte[]>(new std::byte[size]);
        UART::println("Allocated ", size, " bytes at ", ptr.get());
        return ptr;
    }
    catch (const std::bad_alloc&)
    {
        UART::println("Failed to allocate ", size, " bytes");
        return nullptr;
    }
}

struct GlobalObject
{
    GlobalObject() { UART::println("GlobalObject constructor"); }
    ~GlobalObject() { UART::println("GlobalObject destructor"); }
};

GlobalObject globalObject;

int applicationMain()
{
    UART::println("Hello from bare-metal C++ (", __cplusplus, ")!");
    auto ptr1 = tryAllocate(1000000);
    auto ptr2 = tryAllocate(100000000);
    auto ptr3 = tryAllocate(20000000);
    return 0;
}
