#pragma once

//STL
#include <vector>
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

class WorkerThread
{
#ifdef RPG_BUILD_TEST
	friend class events::WorkerThreadTest;
#endif
public:
    ~WorkerThread();

    template<typename ...Args>
    static void newWork(std::function<void(Args...)> work, Args... arguments);
    static void newWork(std::function<void()> work);


private:
    WorkerThread() = default;

    static WorkerThread instance;
    static void worker(std::shared_ptr<AbstractWork> firstWork);

    static inline std::list<std::shared_ptr<AbstractWork>> m_waitingList;

    std::vector<std::thread> m_workers;
    static inline unsigned int m_activeThreads = 0;
    static const inline unsigned int maxThreads = std::thread::hardware_concurrency();

    static const inline unsigned int m_expirationTimeMS = 1000;

    static inline std::mutex m_mutex;
};




template<typename ...Args>
void WorkerThread::newWork(std::function<void(Args...)> work, Args... arguments)
{
    if (m_activeThreads >= maxThreads)
        m_waitingList.push_back(std::make_shared<Work<Args...>>(Work<Args...>(work, arguments...)));
    else
    {
        instance.m_workers.push_back(std::thread(worker, std::make_shared<Work<Args...>>(Work<Args...>(work, arguments...))));
    }
}




} // namespace events

