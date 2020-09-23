#include "CharacterTest.hpp"
#include <Character.hpp>
#include <Inventory.hpp>
#include <Object.hpp>
#include <general_config.hpp>

#include <unit_tests/ContextTest.hpp>

#include <Database.hpp>
#include <Money.hpp>

#ifdef BUILD_USE_FILESYSTEM
#include <filesystem>
#endif

namespace character
{

#ifdef BUILD_USE_FILESYSTEM
/*
 * Database model verification
 */
TEST_F(CharacterTest, VerifyDatabaseModel)
{
    ASSERT_TRUE(Character::verifyDatabaseModel(database));
}

/*
 * Test the well loading of the character from the database
 */
TEST_F(CharacterTest, LoadingCharacterFromDatabase)
{
    using ::testing::Return;
    config::ContextMock* context = new config::ContextMock;
    EXPECT_CALL((*context), kMapPath()).WillRepeatedly(Return("data"));
    auto contextPtr = std::shared_ptr<config::Context>
                      (static_cast<config::Context*>(context));
    Character ch("Brian", contextPtr);

    // Load from the database
    ASSERT_TRUE(ch.loadFromDatabase(database));

    // Test the position
    EXPECT_EQ(ch.position().x(), 1);
    EXPECT_EQ(ch.position().y(), 2);
    EXPECT_EQ(ch.position().z(), 3);

    // Can't continue if the inventory is null
    ASSERT_TRUE(ch.m_inventory);

    // Inventory loading verification
    ASSERT_EQ(ch.m_inventory->size(), 3);
    EXPECT_EQ(ch.m_inventory->get(0)->name(), "object1");
    EXPECT_EQ(ch.m_inventory->get(1)->name(), "object2");
    EXPECT_EQ(ch.m_inventory->get(2)->name(), "object2");

}

/*
 * Test the creation of the database model linked to the Character
 */
TEST_F(CharacterTest, CreatingDatabaseModel)
{
    std::filesystem::path usedFile = "data/sample0.db";
    std::filesystem::remove(usedFile);
    std::shared_ptr<databaseTools::Database> db(new databaseTools::Database(
                usedFile));

    EXPECT_TRUE(Character::createDatabaseModel(db));

}

/**
 * @brief Prepare the database usage
 */
void CharacterTest::SetUp()
{
    object::Money::initialize("bronze",
                              std::pair<std::string, unsigned int>("argent", 100),
                              std::pair<std::string, unsigned int>("or", 50000));
    std::filesystem::path usedFile = "data/sample1.db";
    std::filesystem::path modelFile = "data/sample1.sqlite";
    std::filesystem::copy(modelFile, usedFile,
                          std::filesystem::copy_options::overwrite_existing);

    database = std::shared_ptr<databaseTools::Database>(new databaseTools::Database(
                   usedFile));
}
#endif
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    google::InitGoogleLogging(argv[0]);
    google::SetVLOGLevel("*", VERBOSE);
    return RUN_ALL_TESTS();
}

