#include "ConfigTest.hpp"
#include "ContextTest.hpp"
namespace config {

TEST_F(ConfigTest, GoodObjectName)
{
    Config conf;
    EXPECT_EQ(conf.className(), "Config");
}

TEST_F(ConfigTest, OpenNonExistentFile)
{
    Config conf;
    EXPECT_FALSE(conf.loadFile("Toto"));
}

TEST_F(ConfigTest, OpenFile)
{
    Config conf;
    EXPECT_TRUE(conf.loadFile("data/sample1.ini"));

}

TEST_F(ConfigTest, GetExistentValues)
{
    Config conf("data/sample1.ini");

    EXPECT_EQ(conf.getValue("Section 1", "a_lovely_number"), "8");
    EXPECT_EQ(conf.getValue("Section 1", "a_string"), "Hello World");
    EXPECT_EQ(conf.getValue("Section 2", "a_negative_number"), "-34");

    EXPECT_EQ(conf.getValue("a_bad_number"), "13");
    EXPECT_EQ(conf.getValue("a_negative_number"), "-34");

    Config confWithoutSections("data/sample2.ini");
    EXPECT_EQ(confWithoutSections.getValue("something"), "I'm here");
}

TEST_F(ConfigTest, GetNonExistentValues)
{
    Config conf("data/sample1.ini");

    EXPECT_EQ(conf.getValue("Section 1", "a_non_existant_value"), "");
    EXPECT_EQ(conf.getValue("nothing"), "");
}

}

int main(int argc, char **argv)
{
    config::ContextTest::m_argv = argv;
    std::cout << argv[0] << std::endl;
    google::InitGoogleLogging(argv[0]);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
