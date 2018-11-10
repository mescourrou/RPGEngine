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
    EXPECT_EQ(context.m_argc, 1);
    EXPECT_STREQ(context.m_argv[0], argv);
}

/*
 * Get the path to the runtime directory
 */
TEST_F(ContextTest, runtimeDirectory)
{
    EXPECT_EQ(m_context->runtimeDirectory(), std::string(m_argv[0]));
    std::cout << m_context->runtimeDirectory() << std::endl;
}

void ContextTest::SetUp()
{
    const ::testing::TestInfo* const test_info = ::testing::UnitTest::GetInstance()->current_test_info();
    auto testName = std::string(test_info->name());

    if (testName != "Initialization")
    {
        m_context = std::make_shared<Context>(Context(1, m_argv));
    }
}

void ContextTest::TearDown()
{
}

}


