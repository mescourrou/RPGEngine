#include "WorkerThreadTest.hpp"
namespace events {

TEST_F(WorkerThreadTest, NewWork)
{
    mutex.lock();
    unsigned int cbCalls = 0;
    auto cb = [&cbCalls]() { cbCalls++; usleep(500); };

    WorkerThread::newWork(cb); // 1
    WorkerThread::newWork(cb); // 2
    WorkerThread::newWork(cb); // 3
    WorkerThread::newWork(cb); // 4
    WorkerThread::newWork(cb); // 5
    WorkerThread::newWork(cb); // 6
    WorkerThread::newWork(cb); // 7
    WorkerThread::newWork(cb); // 8
    WorkerThread::newWork(cb); // 9
    WorkerThread::newWork(cb); // 10

    usleep(100000);

    EXPECT_EQ(cbCalls, 10);
    mutex.unlock();
}

TEST_F(WorkerThreadTest, WaitForJoin)
{
    mutex.lock();
    bool active = false;
    auto cb = [&]() {
        active = true;
        usleep(1000);
        active = false;
    };

    WorkerThread::newWork(cb);
    usleep(50);
    EXPECT_TRUE(active);
    WorkerThread::waitForJoin();
    EXPECT_FALSE(active);
    mutex.unlock();
}

}


