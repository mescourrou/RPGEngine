#include "ActionTest.hpp"
#include <ActionHandler.hpp>

namespace events
{

TEST_F(ActionTest, AddAction)
{
    bool executed = false;
    ActionHandler::addAction("Test action", [&executed]()
    {
        executed = true;
    });

    ActionHandler::execute("Test action");
    WorkerThread::waitForJoin();
    EXPECT_TRUE(executed);
}


} // namespace events
