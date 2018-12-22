#include "InventoryTest.hpp"
#include <Inventory.hpp>
#include <Object.hpp>

namespace object {

/*
 * Test the good initialization of the inventory : empty
 */
TEST_F(InventoryTest, Empty)
{
    Inventory iv;

    EXPECT_EQ(iv.size(), 0);
    EXPECT_FALSE(iv.get(0));
    EXPECT_FALSE(iv.get("Something"));
}

/*
 * Test the add of a new object
 */
TEST_F(InventoryTest, Push)
{
    inventory->push(std::make_shared<Object>(Object("A new object")));
    EXPECT_EQ(inventory->size(), 4);
}

/*
 * Test getting inventory object
 */
TEST_F(InventoryTest, Get)
{
    // Get by index
    EXPECT_TRUE(inventory->get(0));
    EXPECT_EQ(inventory->get(0)->name(), "Something");
    EXPECT_TRUE(inventory->get(1));
    EXPECT_EQ(inventory->get(1)->name(), "Something very cool");
    EXPECT_TRUE(inventory->get(2));
    EXPECT_EQ(inventory->get(2)->name(), "Trash");
    EXPECT_FALSE(inventory->get(3));

    // Get by name
    EXPECT_TRUE(inventory->get("Something"));
    EXPECT_TRUE(inventory->get("Something very cool"));
    EXPECT_TRUE(inventory->get("Trash"));
    EXPECT_FALSE(inventory->get("Something which doesnt exists"));
}

/*
 * Test poping object
 */
TEST_F(InventoryTest, Pop)
{
    EXPECT_EQ(inventory->size(), 3);

    // Get by index
    auto object = inventory->pop(0);
    EXPECT_TRUE(object);
    EXPECT_EQ(object->name(), "Something");
    EXPECT_EQ(inventory->size(), 2);

    // Get by name
    EXPECT_FALSE(inventory->pop("Something"));
    EXPECT_EQ(inventory->size(), 2);
    object = inventory->pop("Something very cool");
    EXPECT_TRUE(object);
    EXPECT_EQ(object->name(), "Something very cool");
    EXPECT_EQ(inventory->size(), 1);
}

/**
 * @brief Set up Inventory test by insert object into it
 *
 * The initialization is not done for the "Empty" test
 */
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


