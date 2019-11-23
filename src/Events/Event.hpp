#pragma once

// STL
#include <vector>
#include <thread>
#include <tuple>

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

    void trigger(Args... arguments) const;
    /**
     * @brief Subscribe the function to the Event
     * @param [in] func Function to subscribe
     */
    void subscribeAsync(std::function<void(Args...)> func)
    {
        m_asyncCallList.push_back(func);
    }
    /**
     * @brief Subscribe the function to the Event
     * @param [in] func Function to subscribe
     */
    void subscribeSync(std::function<void(Args...)> func)
    {
        m_syncCallList.push_back(func);
    }

    /**
     * @brief Subscribe the function to the Event
     * @param [in] func Function to subscribe
     * @param[in] instance Instace of the object to call
     */
    template<typename T, typename M>
    void subscribeAsync(T* instance, M func)
    {
        m_asyncCallList.push_back([instance, func](Args... args)
        {
            std::bind(func, instance, args...)();
        });
    }
    /**
     * @brief Subscribe the function to the Event
     * @param [in] func Function to subscribe
     * @param[in] instance Instace of the object to call
     */
    template<typename T, typename M>
    void subscribeSync(T* instance, M func)
    {
        m_syncCallList.push_back([instance, func](Args... args)
        {
            std::bind(func, instance, args...)();
        });
    }

  private:
    std::vector<std::function<void(Args...)>>
                                           m_asyncCallList; ///< List of functions to call
    std::vector<std::function<void(Args...)>>
                                           m_syncCallList; ///< List of functions to call
};

/**
 * @brief Trigger all the function which have subscribe
 * @param [in] arguments Arguments to pass to the functions
 */
template<typename... Args>
void Event<Args...>::trigger(Args... arguments) const
{
    for (auto& call : m_asyncCallList)
    {
        WorkerThread::newWork(call, arguments...);
    }
    for (auto& call : m_syncCallList)
    {
        call(arguments...);
    }

}

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
        for (auto& call : m_asyncCallList)
        {
            WorkerThread::newWork(call);
        }
        for (auto& call : m_syncCallList)
        {
            call();
        }
    }
    /**
     * @brief Subscribe the function to the Event
     * @param [in] func Function to subscribe
     */
    void subscribeAsync(const std::function<void(void)>& func)
    {
        m_asyncCallList.push_back(func);
    }
    /**
     * @brief Subscribe the function to the Event
     * @param [in] func Function to subscribe
     */
    void subscribeSync(const std::function<void(void)>& func)
    {
        m_syncCallList.push_back(func);
    }

    /**
     * @brief Subscribe the function to the Event
     * @param [in] func Function to subscribe
     * @param[in] instance Instace of the object to call
     */
    template<typename T, typename M>
    void subscribeAsync(T* instance, M func)
    {
        m_asyncCallList.push_back([instance, func]()
        {
            std::bind(func, instance)();
        });
    }
    /**
     * @brief Subscribe the function to the Event
     * @param [in] func Function to subscribe
     * @param[in] instance Instace of the object to call
     */
    template<typename T, typename M>
    void subscribeSync(T* instance, M func)
    {
        m_syncCallList.push_back([instance, func]()
        {
            std::bind(func, instance)();
        });
    }

  private:
    std::vector<std::function<void(void)>>
                                        m_asyncCallList; ///< List of functions to call
    std::vector<std::function<void(void)>>
                                        m_syncCallList; ///< List of functions to call
};


} // namespace events

