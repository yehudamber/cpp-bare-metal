#include <cstdlib>
#include <exception>

#include "main.hpp"
#include "uart.hpp"

extern "C" [[noreturn]] void _runtimeMain() noexcept
try
{
    auto exitStatus = applicationMain();
    UART::println("Application exited with status: ", exitStatus);
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