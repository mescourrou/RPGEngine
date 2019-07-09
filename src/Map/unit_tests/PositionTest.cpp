#include "PositionTest.hpp"

#include <general_config.hpp>
// Stl
#ifdef BUILD_USE_FILESYSTEM
#include <filesystem>
#endif

// Project
#include <Position.hpp>
#include <Map.hpp>
#include <Database.hpp>
#include <Character.hpp>

namespace map {

/*
 * Test class name
 */
TEST_F(PositionTest, ClassName)
{
    Position pos;
    EXPECT_EQ(pos.className(), "Position");
}

/*
 * Test the constructors
 */
TEST_F(PositionTest, Constructors)
{
    Position zero;
    EXPECT_EQ(zero.x(), 0);
    EXPECT_EQ(zero.y(), 0);
    EXPECT_EQ(zero.z(), 0);

    Position withoutMap(2,5,3);
    EXPECT_EQ(withoutMap.x(), 2);
    EXPECT_EQ(withoutMap.y(), 5);
    EXPECT_EQ(withoutMap.z(), 3);

    Map map({}, "France");
    Position withMap(std::make_shared<Map>(map), 3,4,2);
    EXPECT_EQ(withMap.x(), 3);
    EXPECT_EQ(withMap.y(), 4);
    EXPECT_EQ(withMap.z(), 2);
    EXPECT_EQ(withMap.map()->name(), "France");

    Position zeroWithMap(std::make_shared<Map>(map));
    EXPECT_EQ(zeroWithMap.x(), 0);
    EXPECT_EQ(zeroWithMap.y(), 0);
    EXPECT_EQ(zeroWithMap.z(), 0);
    EXPECT_EQ(zeroWithMap.map()->name(), "France");
}

/*
 * Test moving the position
 */
TEST_F(PositionTest, Move)
{
    Position pos(2,3,4);
    Vector<3> move{2,-3,1};

    EXPECT_EQ(pos.x(), 2);
    EXPECT_EQ(pos.y(), 3);
    EXPECT_EQ(pos.z(), 4);

    pos += move;
    EXPECT_EQ(pos.x(), 4);
    EXPECT_EQ(pos.y(), 0);
    EXPECT_EQ(pos.z(), 5);
}

/*
 * Equality test
 */
TEST_F(PositionTest, Equal)
{
    // Without map
    {
    Position posRef(2,3,5);
    Position posDiff(4,-3,5);
    Position posEqual(2,3,5);

    EXPECT_TRUE(posRef == posEqual);
    EXPECT_FALSE(posRef == posDiff);
    EXPECT_FALSE(posRef != posEqual);
    EXPECT_TRUE(posRef != posDiff);
    }
    // With Same Map
    Map map({}, "France");
    {
    Position posRef(std::make_shared<Map>(map),2,3,5);
    Position posDiff(std::make_shared<Map>(map),4,-3,5);
    Position posEqual(std::make_shared<Map>(map),2,3,5);

    EXPECT_TRUE(posRef == posEqual);
    EXPECT_FALSE(posRef == posDiff);
    EXPECT_FALSE(posRef != posEqual);
    EXPECT_TRUE(posRef != posDiff);
    }

    Map map2({}, "Spain");
    {
    Position posRef(std::make_shared<Map>(map),2,3,5);
    Position posDiff(std::make_shared<Map>(map2),2,3,5);
    Position posEqual(2,3,5);

    EXPECT_TRUE(posRef == posEqual);
    EXPECT_FALSE(posRef == posDiff);
    EXPECT_FALSE(posRef != posEqual);
    EXPECT_TRUE(posRef != posDiff);
    }
}

/*
 * Test of the distance
 */
TEST_F(PositionTest, Distance)
{
    Position pos1(2,4,6);
    Position pos2(9,-3,3);

    EXPECT_EQ(pos1 - pos2, sqrt(pow(9-2, 2) + pow(-3-4, 2) + pow(3 - 6, 2)));
}

#ifdef BUILD_USE_FILESYSTEM
/*
 * Test the creation of the database model
 */
TEST_F(PositionTest, CreatingDatabaseModel)
{
    std::filesystem::path usedFile = "data/sample0.db";
    std::filesystem::remove(usedFile);
    std::shared_ptr<database::Database> db(new database::Database(usedFile));

    character::Character::createDatabaseModel(db);

    EXPECT_TRUE(Position::createDatabaseModel(db));

}
#endif
}


