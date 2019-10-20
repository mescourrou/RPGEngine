#include "ToolsTest.hpp"

TEST(Tools, SnakeCase)
{
    EXPECT_EQ(Tools::snakeCase("Hello World"), "Hello_World");
    EXPECT_EQ(Tools::snakeCase("Hello World, how are_you?"),
              "Hello_World,_how_are_you?");
}

int main(int argc, char** argv)
{
    google::InitGoogleLogging(argv[0]);
    google::LogToStderr();
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

