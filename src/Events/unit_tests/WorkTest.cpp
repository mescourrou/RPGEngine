#include "WorkTest.hpp"
namespace events {

void callback1()
{
    WorkTest::callEffectuated1 = true;
}

void callback2(int arg1)
{
    if (arg1 == 2)
        WorkTest::callEffectuated2 = true;
}

void callback3(std::string bla, double number)
{
    if (bla == "bla" && number == 4.2)
        WorkTest::callEffectuated3 = true;
}

TEST_F(WorkTest, CallWithZeroArgs)
{
    Work<void> w(callback1);

    callEffectuated1 = false;
    w.run();
    EXPECT_TRUE(callEffectuated1);
}

TEST_F(WorkTest, CallWithOneArgs)
{
    Work<int> w(callback2, 2);

    callEffectuated2 = false;
    w.run();
    EXPECT_TRUE(callEffectuated2);
}

TEST_F(WorkTest, CallWithTwoArgs)
{
    Work<std::string, double> w(callback3, "bla", 4.2);

    callEffectuated3 = false;
    w.run();
    EXPECT_TRUE(callEffectuated3);
}

}


