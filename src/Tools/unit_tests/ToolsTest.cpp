#include "ToolsTest.hpp"

int main(int argc, char **argv)
{
    google::InitGoogleLogging(argv[0]);
    google::LogToStderr();
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

