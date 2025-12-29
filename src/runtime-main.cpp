#include <cstdlib>
#include <exception>

#include "uart.hpp"

void applicationMain();

extern "C" [[noreturn]] void runtimeMain() noexcept
try
{
    applicationMain();
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