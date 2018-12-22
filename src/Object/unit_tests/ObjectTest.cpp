#include "ObjectTest.hpp"
#include <Object.hpp>
#include <glog/logging.h>

namespace object {

/*
 * Test class name
 */
TEST_F(ObjectTest, ClassName)
{
    Object myObject;
    EXPECT_EQ(myObject.className(), "Object");
}

/*
 * Testing default values
 */
TEST_F(ObjectTest, DefaultValues)
{
    Object myObject;
    EXPECT_EQ(myObject.name(), "Unkown object");
    EXPECT_EQ(myObject.description(), "");
}

/*
 * Test stream operator
 */
TEST_F(ObjectTest, StreamOperator)
{
    Object myObject;
    myObject.setName("Screw-driver");
    myObject.setDescription("Very interesting object");

    std::stringstream ss;
    ss << "Object : Screw-driver" << std::endl;
    ss << "Very interesting object" << std::endl;
    ::testing::internal::CaptureStdout();
    std::cout << myObject;
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, ss.str());

}

/**
 * @brief Initialize money system for object using
 */
void ObjectTest::SetUp()
{
    Money::initialize("bronze",
                      std::pair<std::string, unsigned int>("argent", 100),
                      std::pair<std::string, unsigned int>("or", 50000));
}

}

int main(int argc, char **argv)
{
    google::InitGoogleLogging(argv[0]);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

