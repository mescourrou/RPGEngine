#include "WorkerThread.hpp"
namespace events {
WorkerThread WorkerThread::instance;

/**
 * @brief Worker main function.
 *
 * Start the firstWork and then start the works of the waiting list
 * @param [in] firstWork First work to start
 */
void WorkerThread::worker(std::shared_ptr<AbstractWork> firstWork)
{
    firstWork->run();
    m_mutex.lock();
    while (m_waitingList.size() > 0)
    {
        auto work = m_waitingList.front();
        m_waitingList.pop_front();
        m_mutex.unlock();
        work->run();
        work.reset();
        m_mutex.lock();
    }
    m_activeThreads--;
    m_mutex.unlock();
}

/**
 * @brief Destructor : wait for all threads to finish
 */
WorkerThread::~WorkerThread()
{
    for (auto& thread : m_workers)
        thread.join();
}

/**
 * @brief Add a new work to the working list. Start it immedialty if it is possible
 * @param [in] work Work to add
 */
void WorkerThread::newWork(std::function<void()> work)
{
    m_mutex.lock();
    if (m_activeThreads >= maxThreads)
        m_waitingList.push_back(std::make_shared<Work<void>>(Work<void>(work)));
    else
    {
        m_activeThreads++;
        instance.m_workers.push_back(std::thread(worker, std::make_shared<Work<void>>(Work<void>(work))));
    }
    m_mutex.unlock();
}

}

