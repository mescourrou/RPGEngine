#include "ContextTest.hpp"
namespace config {

char** ContextTest::m_argv = nullptr;

TEST_F(ContextTest, Initialization)
{
    char* argv = "PathToRuntime";

    EXPECT_FALSE(Context::get());
    Context::initialize(1, &argv);
    EXPECT_TRUE(Context::get());
    EXPECT_EQ(Context::m_instance->m_argc, 1);
    EXPECT_STREQ(Context::m_instance->m_argv[0], argv);
}

TEST_F(ContextTest, CurrentDirectory)
{
    EXPECT_EQ(Context::get()->currentDirectory(), std::string(m_argv[0]));
}

void ContextTest::SetUp()
{
    const ::testing::TestInfo* const test_info = ::testing::UnitTest::GetInstance()->current_test_info();
    auto testName = std::string(test_info->name());

    if (testName != "Initialization")
    {
        Context::initialize(1, m_argv);
    }
}

void ContextTest::TearDown()
{
    Context::kill();
}

}


