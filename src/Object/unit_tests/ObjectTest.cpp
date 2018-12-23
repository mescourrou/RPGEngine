#include "ObjectTest.hpp"
#include <Object.hpp>
#include <filesystem>
#include <Database.hpp>
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
 * Test the object loading from database
 */
TEST_F(ObjectTest, LoadFromDatabase)
{
    std::filesystem::path modelFile = "data/sample2.sqlite";
    std::filesystem::path useFile = "data/sample2.db";
    std::filesystem::copy(modelFile, useFile, std::filesystem::copy_options::overwrite_existing);

    std::shared_ptr<database::Database> db(new database::Database(useFile));

    Object myObject("myObject");
    ASSERT_TRUE(myObject.loadFromDatabase(db));

    EXPECT_EQ(myObject.value().convertToBaseMoney(), 100);

}

/*
 * Test the creation of an object from database
 */
TEST_F(ObjectTest, CreateFromDatabaseObject)
{
    std::filesystem::path modelFile = "data/sample2.sqlite";
    std::filesystem::path useFile = "data/sample2.db";
    std::filesystem::copy(modelFile, useFile, std::filesystem::copy_options::overwrite_existing);

    std::shared_ptr<database::Database> db(new database::Database(useFile));

    std::shared_ptr<Object> object = Object::createFromDatabase("myObject", db);

    ASSERT_TRUE(object);
    EXPECT_EQ(object->value().convertToBaseMoney(), 100);

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

