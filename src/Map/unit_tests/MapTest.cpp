#include "MapTest.hpp"
#include <fstream>
namespace map {

TEST_F(MapTest, ClassName)
{
    Map map({}, "Somewhere");
    EXPECT_EQ(map.className(), "Map");
}

TEST_F(MapTest, Init)
{
    Map map({}, "Somewhere");
    EXPECT_EQ(map.name(), "Somewhere");
}

TEST_F(MapTest, LoadCollisionLayer)
{
    using ::testing::Return;
    config::ContextMock* context = new config::ContextMock;
    EXPECT_CALL((*context), kMapPath()).WillOnce(Return("data"));
    auto contextPtr = std::shared_ptr<config::Context>(static_cast<config::Context*>(context));
    Map map(contextPtr, "Map1");
    ASSERT_TRUE(map.load("Map1"));

    EXPECT_TRUE(map.collision(Vector<2>{950,1390}));
    EXPECT_TRUE(map.collision(Vector<2>{1141,1328}));
    EXPECT_TRUE(map.collision(Vector<2>{1279,1289}));
    EXPECT_FALSE(map.collision(Vector<2>{1232,1334}));
    EXPECT_FALSE(map.collision(Vector<2>{1260,1301}));

}

TEST_F(MapTest, JsonTests)
{
    json myJson;
    std::ifstream file("data/Map1.json");
    ASSERT_TRUE(file.is_open());

    file >> myJson;

    ASSERT_TRUE(myJson.is_object());
    auto layers = myJson["layers"];

    ASSERT_TRUE(layers.is_array());
    for (auto layer : layers)
    {
        if (layer["name"] == "collisions")
        {
            std::cout << "Found collisions" << std::endl;
            for (auto object : layer["objects"])
            {
                if (object["visible"].get<bool>())
                    std::cout << "Polygone visible" << std::endl;
                else
                    std::cout << "Polygone invisible" << std::endl;
            }
        }

    }

}

}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}

