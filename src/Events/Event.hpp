#pragma once

// STL
#include <vector>
#include <thread>

// Project
#include "general_config.hpp"
#include <BaseObject.hpp>
#include <WorkerThread.hpp>

#ifdef RPG_BUILD_TEST
#include <gtest/gtest.h>
#endif

namespace events
{

#ifdef RPG_BUILD_TEST
class EventTest;
#endif

template <typename ...Args>
class Event
{
#ifdef RPG_BUILD_TEST
    friend class events::EventTest;
#endif
public:
    Event() = default;
    ~Event() = default;

    void trigger(Args... arguments) const
    {
        for (auto& call : m_callList)
        {
            WorkerThread::newWork(call, arguments...);
        }
    }
    void subscribe(std::function<void(Args...)> func)
    {
        m_callList.push_back(func);
    }
    /*void subscribe(BaseObject* object, std::function<void(Args...)> func)
    {
        m_callList.push_back(std::bind(func, object, PLACEHOLDER(m_kNumberOfArgs)));
    }*/

private:
    std::vector<std::function<void(Args...)>> m_callList;
};

template <>
class Event<void>
{
#ifdef RPG_BUILD_TEST
    friend class events::EventTest;
#endif
public:
    Event() = default;
    ~Event() = default;

    void trigger() const
    {
        for (auto& call : m_callList)
        {
            WorkerThread::newWork(call);
        }
    }
    void subscribe(std::function<void(void)> func)
    {
        m_callList.push_back(func);
    }

private:
    std::vector<std::function<void(void)>> m_callList;
};


} // namespace events

