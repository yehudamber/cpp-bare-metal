#include <cstdlib>
#include <exception>

#include "uart.hpp"

void bmain();

extern "C" [[noreturn]] void callMain() noexcept
try
{
    bmain();
    UART::println("Shutting down...");
    std::exit(0);
}
catch (const std::exception& e)
{
    UART::println("Uncaught exception: ", e.what(), "\nShutting down...");
    std::exit(0);
}
catch (...)
{
    UART::println("Uncaught unknown exception\n"
                  "Shutting down...");
    std::exit(0);
}