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

/**
 * @namespace events
 * @brief Group the class for managing events and signals
 */
namespace events
{

#ifdef RPG_BUILD_TEST
class EventTest;
#endif

/**
 * @class Event
 * @brief Handle subsciption to an event and triggering it
 *
 * See Event<void> for event without arguments
 *
 * Example:
 * @code
 * Event<int> event;
 * bool argCb = false;
 * auto cb = [&argCb] (int number) {
 *     if (number == 3)
 *         argCb = true;
 * };
 *
 * event.subscribe(cb);
 *
 * event.trigger(3);
 * @endcode
 *
 * With an object method, you need to use std::bin.
 * @code
 * Event<std::string> event;
 * AClass o;
 *
 * e2.subscribe(std::bind(&AClass::argMethod, &o, std::placeholders::_1));
 *
 * e2.trigger("Hello");
 * @endcode
 */
template <typename ...Args>
class Event
{
#ifdef RPG_BUILD_TEST
    friend class events::EventTest;
#endif
public:
    /**
     * @fn Event()
     * @brief Default constructor
     */
    Event() = default;
    /**
     * @fn ~Event()
     * @brief Default destructor
     */
    ~Event() = default;

    /**
     * @brief Trigger all the function which have subscribe
     * @param [in] arguments Arguments to pass to the functions
     */
    void trigger(Args... arguments) const
    {
        for (auto& call : m_callList)
        {
            WorkerThread::newWork(call, arguments...);
        }

    }
    /**
     * @brief Subscribe the function to the Event
     * @param [in] func Function to subscribe
     */
    void subscribe(std::function<void(Args...)> func)
    {
        m_callList.push_back(func);
    }

private:
    std::vector<std::function<void(Args...)>> m_callList; ///< List of functions to call
};

/**
 * @code
 * Event<void> event;
 * bool noArgCb = false;
 * auto cb = [&noArgCb] () {
 *     noArgCb = true;
 * };
 *
 * event.subscribe(cb);
 *
 * event.trigger();
 * @endcode
 *
 */
template <>
class Event<void>
{
#ifdef RPG_BUILD_TEST
    friend class events::EventTest;
#endif
public:
    Event() = default;
    ~Event() = default;

    /**
     * @brief Trigger all the function which have subscribe
     */
    void trigger() const
    {
        for (auto& call : m_callList)
        {
            WorkerThread::newWork(call);
        }
    }
    /**
     * @brief Subscribe the function to the Event
     * @param [in] func Function to subscribe
     */
    void subscribe(std::function<void(void)> func)
    {
        m_callList.push_back(func);
    }

private:
    std::vector<std::function<void(void)>> m_callList; ///< List of functions to call
};


} // namespace events

