#include "WorkerThreadTest.hpp"
namespace events {

TEST_F(WorkerThreadTest, Test)
{
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
}

}


