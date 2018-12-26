#include "CharacterTest.hpp"
#include <Character.hpp>
#include <Inventory.hpp>
#include <Object.hpp>

#include <Database.hpp>
#include <Money.hpp>

#include <filesystem>

namespace character {

TEST_F(CharacterTest, VerifyDatabaseModel)
{
    ASSERT_TRUE(Character::verifyDatabaseModel(database));
}

TEST_F(CharacterTest, LoadingCharacterFromDatabase)
{
    Character ch("Brian");

    ASSERT_TRUE(ch.loadFromDatabase(database));

    EXPECT_EQ(ch.position().x(), 1);
    EXPECT_EQ(ch.position().y(), 2);
    EXPECT_EQ(ch.position().z(), 3);

    ASSERT_TRUE(ch.m_inventory);

    ASSERT_EQ(ch.m_inventory->size(), 3);
    EXPECT_EQ(ch.m_inventory->get(0)->name(), "object1");
    EXPECT_EQ(ch.m_inventory->get(1)->name(), "object2");
    EXPECT_EQ(ch.m_inventory->get(2)->name(), "object2");

}

void CharacterTest::SetUp()
{
    object::Money::initialize("bronze",
                      std::pair<std::string, unsigned int>("argent", 100),
                      std::pair<std::string, unsigned int>("or", 50000));
    std::filesystem::path usedFile = "data/sample1.db";
    std::filesystem::path modelFile = "data/sample1.sqlite";
    std::filesystem::copy(modelFile, usedFile, std::filesystem::copy_options::overwrite_existing);

    database = std::shared_ptr<database::Database>(new database::Database(usedFile));
}

}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    google::SetVLOGLevel("*", VERBOSE);
    return RUN_ALL_TESTS();
}

