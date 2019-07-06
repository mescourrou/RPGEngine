#include "ConfigTest.hpp"
#include "ContextTest.hpp"

#include <glog/logging.h>

#include <Config.hpp>

namespace config {

/*
 * Classname test
 */
TEST_F(ConfigTest, GoodObjectName)
{
    Config conf;
    EXPECT_EQ(conf.className(), "Config");
}

/*
 * What happen if the file given doesn't exists
 */
TEST_F(ConfigTest, OpenNonExistentFile)
{
    Config conf;
    EXPECT_FALSE(conf.loadFile("Toto"));
}

/*
 * Load existent file
 */
TEST_F(ConfigTest, OpenFile)
{
    Config conf;
    EXPECT_TRUE(conf.loadFile("data/sample1.ini"));

}

/*
 * Getting existing values
 */
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

/*
 * Test if return empty string when asking for an inexistant value
 */
TEST_F(ConfigTest, GetNonExistentValues)
{
    Config conf("data/sample1.ini");

    EXPECT_EQ(conf.getValue("Section 1", "a_non_existant_value"), "");
    EXPECT_EQ(conf.getValue("nothing"), "");
}

/*
 * Test getting all values when the value is non existant
 */
TEST_F(ConfigTest, GetAllValuesEmpty)
{
    Config conf("data/sample1.ini");

    EXPECT_EQ(conf.getAllValues("Section 3", "a_non_existant_value").size(), 0);
}

/*
 * Test getting all the values of a couple section+key
 */
TEST_F(ConfigTest, GetAllValues)
{
    Config conf("data/sample1.ini");

    auto values = conf.getAllValues("Section 3", "module");
    ASSERT_EQ(values.size(), 3);

    EXPECT_EQ(values.at(0), "mod1");
    EXPECT_EQ(values.at(1), "mod2");
    EXPECT_EQ(values.at(2), "mod3");
}

/*
 * Test getting the section list
 */
TEST_F(ConfigTest, GetAllSections)
{
    Config conf("data/sample1.ini");

    auto sections = conf.getAllSections();
    ASSERT_EQ(sections.size(), 3);

    EXPECT_EQ(sections.at(0), "Section 1");
    EXPECT_EQ(sections.at(1), "Section 2");
    EXPECT_EQ(sections.at(2), "Section 3");
}

}

int main(int argc, char **argv)
{
    config::ContextTest::m_argv = argv;
    std::cout << argv[0] << std::endl;
    google::InitGoogleLogging(argv[0]);
    google::LogToStderr();
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
