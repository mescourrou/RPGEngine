#include "InventoryTest.hpp"
namespace object {

TEST_F(InventoryTest, Empty)
{
    Inventory iv;

    EXPECT_EQ(iv.size(), 0);
    EXPECT_FALSE(iv.get(0));
    EXPECT_FALSE(iv.get("Something"));
}

TEST_F(InventoryTest, Push)
{
    inventory->push(std::make_shared<Object>(Object("A new object")));
    EXPECT_EQ(inventory->size(), 4);
}

TEST_F(InventoryTest, Get)
{
    EXPECT_TRUE(inventory->get(0));
    EXPECT_TRUE(inventory->get(1));
    EXPECT_TRUE(inventory->get(2));
    EXPECT_FALSE(inventory->get(3));

    EXPECT_TRUE(inventory->get("Something"));
    EXPECT_TRUE(inventory->get("Something very cool"));
    EXPECT_TRUE(inventory->get("Trash"));
    EXPECT_FALSE(inventory->get("Something which doesnt exists"));
}

TEST_F(InventoryTest, Pop)
{
    EXPECT_EQ(inventory->size(), 3);
    EXPECT_TRUE(inventory->pop(0));
    EXPECT_EQ(inventory->size(), 2);

    EXPECT_FALSE(inventory->pop("Something"));
    EXPECT_EQ(inventory->size(), 2);
    EXPECT_TRUE(inventory->pop("Something very cool"));
    EXPECT_EQ(inventory->size(), 1);
}

void InventoryTest::SetUp()
{
    Money::initialize("bronze",
                      std::pair<std::string, unsigned int>("argent", 100),
                      std::pair<std::string, unsigned int>("or", 50000));

    const ::testing::TestInfo* const test_info = ::testing::UnitTest::GetInstance()->current_test_info();
    auto testName = std::string(test_info->name());

    if (testName != "Empty")
    {
        inventory.reset(new Inventory);
        inventory->m_inventory.push_back(std::make_shared<Object>(Object("Something")));
        inventory->m_inventory.push_back(std::make_shared<Object>(Object("Something very cool")));
        inventory->m_inventory.push_back(std::make_shared<Object>(Object("Trash")));
    }
}

}


