#include "MapTest.hpp"
#include <fstream>
#include <unit_tests/ContextTest.hpp>
namespace map
{

/*
 * Test the classname
 */
TEST_F(MapTest, ClassName)
{
    Map map({}, "Somewhere");
    EXPECT_EQ(map.className(), "Map");
}

/*
 * Test init
 */
TEST_F(MapTest, Init)
{
    Map map({}, "Somewhere");
    EXPECT_EQ(map.name(), "Somewhere");
}

/*
 * Test to load a collision layer
 */
TEST_F(MapTest, LoadCollisionLayer)
{
    using ::testing::Return;
    config::ContextMock* context = new config::ContextMock;
    EXPECT_CALL((*context), kMapPath()).WillOnce(Return("data"));
    auto contextPtr = std::shared_ptr<config::Context>
                      (static_cast<config::Context*>(context));
    Map map(contextPtr, "Map1");
    ASSERT_TRUE(map.load());

    EXPECT_TRUE(map.collision(Vector<2> {950, 1390}));
    EXPECT_TRUE(map.collision(Vector<2> {1141, 1328}));
    EXPECT_TRUE(map.collision(Vector<2> {1279, 1289}));
    EXPECT_FALSE(map.collision(Vector<2> {1232, 1334}));
    EXPECT_FALSE(map.collision(Vector<2> {1260, 1301}));

}

}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}

