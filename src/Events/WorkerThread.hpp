#pragma once

//STL
#include <list>
#include <thread>
#include <mutex>

// Project
#include "general_config.hpp"
#include <BaseObject.hpp>
#include <Work.hpp>

#ifdef RPG_BUILD_TEST
#include <gtest/gtest.h>
#endif

namespace events
{

#ifdef RPG_BUILD_TEST
class WorkerThreadTest;
#endif

/**
 * @brief Manage Work and allocate them to separated threads.
 *
 * The number of thread is limited by the number of thread of the device.
 */
class WorkerThread
{
#ifdef RPG_BUILD_TEST
    friend class events::WorkerThreadTest;
#endif
  public:
    ~WorkerThread();

    template<typename ...Args>
    static void newWork(const std::function<void(Args...)>& work,
                        Args... arguments);
    static void newWork(const std::function<void()>& work);

    template<typename I, typename M, typename ...Args>
    static void newWork(I* instance, M func, Args... arguments);
    template<typename I, typename M>
    static void newWork(I* instance, M func);

    static void waitForJoin();


  private:
    /// @brief Constructor
    WorkerThread() = default;

    static WorkerThread instance; ///< Singleton instance
    static void worker(std::shared_ptr<AbstractWork> firstWork);

    /**
     * @brief Work waiting list.
     *
     * It is only used if there is not enough thread available at the moment
     */
    static inline std::list<std::shared_ptr<AbstractWork>> m_waitingList;

    std::vector<std::thread> m_workers; ///< Thread list
    static inline unsigned int m_activeThreads = 0; ///< Number of active threads
    static const inline unsigned int maxThreads =
        std::thread::hardware_concurrency(); ///< Maximum number of threads

    static const inline unsigned int m_expirationTimeMS =
        1000; ///< Time to wait before closing a inactive thread

    static inline std::mutex m_mutex; ///< Mutex
};

/**
 * @fn void WorkerThread::newWork(std::function<void(Args...)> work, Args... arguments)
 * @brief Add a new work
 *
 * @param work Function to call for work
 * @param arguments Arguments of the function
 */
template<typename ...Args>
void WorkerThread::newWork(const std::function<void(Args...)>& work,
                           Args... arguments)
{
    m_mutex.lock();
    if (m_activeThreads >= maxThreads)
        m_waitingList.push_back(std::make_shared<Work<Args...>>(Work<Args...>(work,
                                arguments...)));
    else
    {
        m_activeThreads++;
        instance.m_workers.push_back(std::thread(worker,
                                     std::make_shared<Work<Args...>>(Work<Args...>(work, arguments...))));
    }
    m_mutex.unlock();
}

template<typename I, typename M, typename... Args>
void WorkerThread::newWork(I* instance, M func, Args ...arguments)
{
    newWork([ = ]()
    {
        std::bind(func, instance, arguments...)();
    });
}

template<typename I, typename M>
void WorkerThread::newWork(I* instance, M func)
{
    newWork([ = ]()
    {
        std::bind(func, instance)();
    });
}



} // namespace events

