#include "GameTest.hpp"

#include <Database.hpp>
#include <Character.hpp>

namespace game {
TEST_F(GameTest, CreatingDatabaseModel)
{
    std::filesystem::path usedFile = "data/sample0.db";
    std::filesystem::remove(usedFile);
    std::shared_ptr<database::Database> db(new database::Database(usedFile));
    character::Character::createDatabaseModel(db);

    EXPECT_TRUE(Game::createDatabaseModel(db));

}
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

