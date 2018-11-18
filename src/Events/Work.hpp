#pragma once

// Project
#include "general_config.hpp"
#include <BaseObject.hpp>

#ifdef RPG_BUILD_TEST
#include <gtest/gtest.h>
#endif

namespace events
{

#ifdef RPG_BUILD_TEST
class WorkTest;
#endif

class AbstractWork
{
#ifdef RPG_BUILD_TEST
	friend class events::WorkTest;
#endif
public:
    AbstractWork() = default;
    virtual ~AbstractWork() = default;

    virtual void run() const = 0;
};

template<typename ...Args>
class Work : public AbstractWork
{
public:
    Work(std::function<void(Args...)> func, Args... arguments) : m_func(func), m_arguments(arguments...) {}
    ~Work() override = default;

    void run() const override
    {
        std::apply(m_func, m_arguments);
    }

private:
    std::function<void(Args...)> m_func;
    std::tuple<Args...> m_arguments;


};

template<>
class Work<void> : public AbstractWork
{
public:
    Work(std::function<void(void)> func) : m_func(func) {}
    ~Work() override = default;
    void run() const override
    {
        m_func();
    }

private:
    std::function<void(void)> m_func;
};

} // namespace events

