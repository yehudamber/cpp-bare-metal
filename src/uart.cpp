#include "uart.hpp"

namespace UART
{

constexpr auto UART_BASE_ADDR = static_cast<std::uintptr_t>(0x10000000);
const auto UART_THR = reinterpret_cast<volatile char*>(UART_BASE_ADDR);
const auto UART_LSR = reinterpret_cast<const volatile std::byte*>(UART_BASE_ADDR + 0x05);

constexpr auto UART_TX_IDLE = static_cast<std::byte>(1 << 5);

void putc(char c) noexcept
{
    while ((*UART_LSR & UART_TX_IDLE) == static_cast<std::byte>(0))
        ;
    *UART_THR = c;
}

}