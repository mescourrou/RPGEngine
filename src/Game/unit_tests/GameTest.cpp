#include "GameTest.hpp"

#ifdef BUILD_USE_FILESYSTEM
#include <filesystem>
#endif

#include <Database.hpp>
#include <Character.hpp>

namespace game {
/*
 * Testing if the creation of the database model is correct
 */
TEST_F(GameTest, CreatingDatabaseModel)
{
#ifdef BUILD_USE_FILESYSTEM
    std::filesystem::path usedFile = "data/sample0.db";
    std::filesystem::remove(usedFile);
    auto db = std::make_shared<database::Database>(usedFile);
    character::Character::createDatabaseModel(db);

    EXPECT_TRUE(Game::createDatabaseModel(db));
#endif

}
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

