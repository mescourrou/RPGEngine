#include "ObjectTest.hpp"
namespace object {

TEST_F(ObjectTest, ClassName)
{
    Object myObject;
    EXPECT_EQ(myObject.className(), "Object");
}

TEST_F(ObjectTest, DefaultValues)
{
    Object myObject;
    EXPECT_EQ(myObject.name(), "Unkown object");
    EXPECT_EQ(myObject.description(), "");
}

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

