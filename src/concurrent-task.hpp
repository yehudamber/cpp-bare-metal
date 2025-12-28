#pragma once

#include <coroutine>
#include <exception>
#include <utility>

class ConcurrentTask
{
    struct YieldTag
    {
    };

public:
    static constexpr YieldTag yield = {};

    class promise_type
    {
    public:
        auto get_return_object() { return ConcurrentTask(*this); }

        std::suspend_always initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }

        void await_transform(auto&&) = delete;

        std::suspend_always yield_value(YieldTag) noexcept { return {}; }

        void return_void() noexcept { }

        void unhandled_exception() noexcept { m_exception = std::current_exception(); }

    private:
        std::exception_ptr m_exception;
        friend ConcurrentTask;
    };

    ConcurrentTask() : m_coroutine(nullptr) { }

    ConcurrentTask(const ConcurrentTask&) = delete;
    void operator=(const ConcurrentTask&) = delete;

    ConcurrentTask(ConcurrentTask&& other) noexcept
        : m_coroutine(std::exchange(other.m_coroutine, nullptr))
    {
    }

    ConcurrentTask& operator=(ConcurrentTask&& other) noexcept
    {
        ConcurrentTask(std::move(other)).swap(*this);
        return *this;
    }

    ~ConcurrentTask()
    {
        if (m_coroutine)
        {
            m_coroutine.destroy();
        }
    }

    void swap(ConcurrentTask& other) noexcept { std::swap(m_coroutine, other.m_coroutine); }

    explicit operator bool() const { return m_coroutine != nullptr; }

    bool resume()
    {
        m_coroutine.resume();
        if (m_coroutine.done())
        {
            if (m_coroutine.promise().m_exception)
            {
                std::rethrow_exception(m_coroutine.promise().m_exception);
            }
            return false;
        }
        return true;
    }

private:
    explicit ConcurrentTask(promise_type& promise)
        : m_coroutine(std::coroutine_handle<promise_type>::from_promise(promise))
    {
    }

    std::coroutine_handle<promise_type> m_coroutine;
};