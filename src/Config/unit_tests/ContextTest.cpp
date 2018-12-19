#include "ContextTest.hpp"
namespace config {

char** ContextTest::m_argv = nullptr;

/*
 * Test if initialization is well done
 */
TEST_F(ContextTest, Initialization)
{
    char* argv = "PathToRuntime";

    Context context(1, &argv);
    EXPECT_EQ(context.kProgramArguments().size(), 0);
    EXPECT_STREQ(context.runtimeDirectory().c_str(), argv);
}

/*
 * Test if initialization is well done with multiples arguments
 */
TEST_F(ContextTest, InitializationWithMultiplesArguments)
{
    char* arg0 = "PathToRuntime", *arg1 = "1st arg", *arg2 = "2nd arg";
    char* argv[3];
    argv[0] = arg0;
    argv[1] = arg1;
    argv[2] = arg2;

    Context context(3, argv);
    EXPECT_EQ(context.kProgramArguments().size(), 2);
    EXPECT_STREQ(context.runtimeDirectory().c_str(), arg0);
    EXPECT_STREQ(context.kProgramArguments().at(0).c_str(), arg1);
    EXPECT_STREQ(context.kProgramArguments().at(1).c_str(), arg2);
}

/*
 * Get the path to the runtime directory
 */
TEST_F(ContextTest, runtimeDirectory)
{
    EXPECT_EQ(m_context->runtimeDirectory(), std::string(m_argv[0]));
}

void ContextTest::SetUp()
{
    const ::testing::TestInfo* const test_info = ::testing::UnitTest::GetInstance()->current_test_info();
    auto testName = std::string(test_info->name());

    m_context = std::make_shared<Context>(Context(1, m_argv));
}

void ContextTest::TearDown()
{
}

}


