#include "WorkerThread.hpp"
namespace events {
WorkerThread WorkerThread::instance;

void WorkerThread::worker(std::shared_ptr<AbstractWork> firstWork)
{
    m_activeThreads++;
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
    m_mutex.unlock();
    m_activeThreads--;
}

WorkerThread::~WorkerThread()
{
    for (auto& thread : m_workers)
        thread.join();
}

void WorkerThread::newWork(std::function<void()> work)
{
    m_mutex.lock();
    if (m_activeThreads >= maxThreads)
        m_waitingList.push_back(std::make_shared<Work<void>>(Work<void>(work)));
    else
    {
        instance.m_workers.push_back(std::thread(worker, std::make_shared<Work<void>>(Work<void>(work))));
    }
    m_mutex.unlock();
}

}

