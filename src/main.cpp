#include <algorithm>
#include <exception>
#include <ranges>
#include <string_view>

#include "uart.hpp"
#include "concurrent-task.hpp"

using namespace std::literals;

ConcurrentTask printer(std::string_view message, int times)
{
    for (auto _ : std::views::iota(0, times))
    {
        UART::println(message);
        co_yield ConcurrentTask::yield;
    }
}

ConcurrentTask thrower()
{
    co_yield ConcurrentTask::yield;
    co_yield ConcurrentTask::yield;
    throw std::exception();
}

void applicationMain()
{
    auto tasks = std::array{ printer("Hello", 6), thrower() };
    UART::println("Created ", tasks.size(), " tasks");
    while (std::ranges::any_of(tasks, &ConcurrentTask::operator bool))
    {
        for (auto& task : tasks)
        {
            if (task)
            {
                if (!task.resume())
                {
                    task = {};
                }
            }
        }
    }
}
