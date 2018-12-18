#include "CharacterTest.hpp"

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
}

void CharacterTest::SetUp()
{
    std::filesystem::path usedFile = "data/sample1.db";
    std::filesystem::path modelFile = "data/sample1.sqlite";
    std::filesystem::copy(modelFile, usedFile, std::filesystem::copy_options::overwrite_existing);

    database = std::shared_ptr<database::Database>(new database::Database(usedFile));
}

}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

