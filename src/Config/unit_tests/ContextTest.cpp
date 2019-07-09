#include "ContextTest.hpp"
#include <general_config.hpp>
#ifdef BUILD_USE_FILESYSTEM
#include <filesystem>
#endif

#include <Context.hpp>

namespace config {

char** ContextTest::m_argv = nullptr;

/*
 * Test if initialization is well done
 */
TEST_F(ContextTest, Initialization)
{
    char* argv = "Path/To/Runtime.sh";

    Context context(1, &argv);
    EXPECT_EQ(context.kProgramArguments().size(), 0);
    EXPECT_STREQ(context.runtimeDirectory().c_str(), "Path/To");
}

/*
 * Test if initialization is well done with multiples arguments
 */
TEST_F(ContextTest, InitializationWithMultiplesArguments)
{
    char* arg0 = "Path/To/Runtime.sh", *arg1 = "1st arg", *arg2 = "2nd arg";
    char* argv[3];
    argv[0] = arg0;
    argv[1] = arg1;
    argv[2] = arg2;

    Context context(3, argv);
    EXPECT_EQ(context.kProgramArguments().size(), 2);
    EXPECT_STREQ(context.runtimeDirectory().c_str(), "Path/To");
    EXPECT_STREQ(context.kProgramArguments().at(0).c_str(), arg1);
    EXPECT_STREQ(context.kProgramArguments().at(1).c_str(), arg2);
}

/*
 * Get the path to the runtime directory
 */
TEST_F(ContextTest, runtimeDirectory)
{
#ifdef BUILD_USE_FILESYSTEM
    std::filesystem::path runtimeDir = std::string(m_argv[0]);
    EXPECT_EQ(m_context->runtimeDirectory(), runtimeDir.parent_path().string());
#else
    std::string runtimeDirectory = m_argv[0];
    auto lastSeparator = runtimeDirectory.find_last_of('/');
    runtimeDirectory = runtimeDirectory.substr(0, lastSeparator);
    EXPECT_EQ(m_context->runtimeDirectory(), runtimeDirectory);
#endif
}

/**
 * @brief Setting the context
 */
void ContextTest::SetUp()
{
    m_context = std::make_shared<Context>(Context(1, m_argv));
}

}


