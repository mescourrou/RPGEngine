#include "InstrumentorTest.hpp"
#include <general_config.hpp>
#include <Instrumentor.hpp>
#include <InstrumentationTimer.hpp>

#include <fstream>
#include <json.hpp>

#ifdef BUILD_USE_FILESYSTEM
#include <filesystem>

TEST_F(InstrumentorTest, Sessions)
{
    const std::string expectedFileName = "testSession_result.json";

    if (std::filesystem::exists(expectedFileName))
        std::filesystem::remove(expectedFileName);

    Instrumentor::get().beginSession("testSession");

    Instrumentor::get().endSession();

    EXPECT_TRUE(std::filesystem::exists(expectedFileName));
}

TEST_F(InstrumentorTest, Timer)
{
    const std::string expectedFileName = "testTimerSession_result.json";

    if (std::filesystem::exists(expectedFileName))
        std::filesystem::remove(expectedFileName);
    Instrumentor::get().beginSession("testTimerSession");
    const std::string expectedName = "timer";
    {
        InstrumentationTimer t(expectedName.c_str());
        for (unsigned int i = 0; i < 100; i++);
    }
    Instrumentor::get().endSession();

    ASSERT_TRUE(std::filesystem::exists(expectedFileName));

    std::ifstream file(expectedFileName);
    ASSERT_TRUE(file.is_open());

    nlohmann::json json;
    file >> json;

    ASSERT_TRUE(json.is_object());
    ASSERT_TRUE(json.contains("traceEvents"));
    auto events = json.at("traceEvents");
    ASSERT_TRUE(events.is_array());
    ASSERT_EQ(events.size(), 1);

    auto e = events.at(0);
    ASSERT_TRUE(e.is_object());
    ASSERT_TRUE(e.contains("name"));
    EXPECT_EQ(e.at("name"), expectedName);

}

#endif
