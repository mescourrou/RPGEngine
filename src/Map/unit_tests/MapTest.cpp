#include "MapTest.hpp"
namespace map {

TEST_F(MapTest, ClassName)
{
    Map map("Somewhere");
    EXPECT_EQ(map.className(), "Map");
}

TEST_F(MapTest, Init)
{
    Map map("Somewhere");
    EXPECT_EQ(map.name(), "Somewhere");
}

}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

