#include <climits>
#include <concepts>
#include <ranges>
#include <string_view>
#include <type_traits>
#include <utility>

namespace UART
{

void putc(char c) noexcept;

template <std::ranges::input_range R>
    requires std::convertible_to<std::ranges::range_reference_t<R>, char>
void putRange(R&& range)
{
    for (char c : range)
    {
        putc(c);
    }
}

inline void print(std::string_view str) noexcept { putRange(str); }

template <std::integral Int, Int base = 10>
    requires(base >= 2 && base <= 16)
void print(Int num) noexcept
{
    using namespace std::literals;
    static constexpr auto digitChars = "0123456789abcdfe"sv;
    static_assert(digitChars.size() >= base);

    if constexpr (std::is_signed_v<Int>)
    {
        if (num < 0)
        {
            putc('-');
            num = -num;
        }
    }

    if (num == 0)
    {
        putc('0');
        return;
    }

    std::array<char, sizeof(Int) * CHAR_BIT> digits;
    auto nextFree = digits.rbegin();
    for (; num > 0; num /= base)
    {
        *nextFree++ = digitChars[num % base];
    }
    putRange(std::ranges::subrange(nextFree.base(), digits.end()));
}

template <typename... Args>
    requires(sizeof...(Args) > 1)
void print(Args&&... args) noexcept((noexcept(UART::print(std::declval<Args>())) && ...))
{
    (UART::print(std::forward<Args>(args)), ...);
}

template <typename... Args>
void println(Args&&... args) noexcept((noexcept(UART::print(std::declval<Args>())) && ...))
{
    (UART::print(std::forward<Args>(args)), ...);
    putc('\n');
}

}