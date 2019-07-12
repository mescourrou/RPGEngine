#include "VendorTest.hpp"
#include <Vendor.hpp>
#include <Database.hpp>
#include <Object.hpp>
#include <unit_tests/ContextTest.hpp>

namespace character {

/*
 * Test the normal buying scenario
 */
TEST_F(VendorTest, Buying)
{
    Vendor vendor("Fishman", context);

    ASSERT_TRUE(vendor.loadFromDatabase(db));
    Character ch("Brian", context);

    // Load from the database
    ASSERT_TRUE(ch.loadFromDatabase(db));

    EXPECT_EQ(vendor.seeInventory().lock()->size(), 10);
    EXPECT_EQ(vendor.seeInventory().lock()->money().convertToBaseMoney(), 10000);
    EXPECT_EQ(ch.m_inventory->money().convertToBaseMoney(), 100);

    EXPECT_EQ(vendor.m_inventory->getNumberOf("object1"), 10);
    EXPECT_EQ(ch.m_inventory->getNumberOf("object1"), 1);
    EXPECT_EQ(ch.m_inventory->size(), 3);

    vendor.sell(0, ch);

    EXPECT_EQ(ch.m_inventory->size(), 4);
    EXPECT_EQ(vendor.m_inventory->getNumberOf("object1"), 9);
    EXPECT_EQ(ch.m_inventory->getNumberOf("object1"), 2);
    EXPECT_EQ(vendor.seeInventory().lock()->money().convertToBaseMoney(), 10010);
    EXPECT_EQ(ch.m_inventory->money().convertToBaseMoney(), 90);

    vendor.sell("object1", ch);

    EXPECT_EQ(ch.m_inventory->size(), 5);
    EXPECT_EQ(vendor.m_inventory->getNumberOf("object1"), 8);
    EXPECT_EQ(ch.m_inventory->getNumberOf("object1"), 3);
}

/*
 * Test the normal selling scenario
 */
TEST_F(VendorTest, Selling)
{
    Vendor vendor("Fishman", context);

    ASSERT_TRUE(vendor.loadFromDatabase(db));
    Character ch("Brian", context);

    // Load from the database
    ASSERT_TRUE(ch.loadFromDatabase(db));

    EXPECT_EQ(vendor.seeInventory().lock()->size(), 10);
    EXPECT_EQ(vendor.seeInventory().lock()->money().convertToBaseMoney(), 10000);
    EXPECT_EQ(ch.m_inventory->money().convertToBaseMoney(), 100);

    EXPECT_EQ(vendor.m_inventory->getNumberOf("object1"), 10);
    EXPECT_EQ(ch.m_inventory->getNumberOf("object1"), 1);
    EXPECT_EQ(ch.m_inventory->size(), 3);

    vendor.buy("object1", ch);

    EXPECT_EQ(ch.m_inventory->size(), 2);
    EXPECT_EQ(vendor.m_inventory->getNumberOf("object1"), 11);
    EXPECT_EQ(ch.m_inventory->getNumberOf("object1"), 0);
    EXPECT_EQ(vendor.seeInventory().lock()->money().convertToBaseMoney(), 9990);
    EXPECT_EQ(ch.m_inventory->money().convertToBaseMoney(), 110);

}

/*
 * Trying to sell an object to a character which has not enough money
 */
TEST_F(VendorTest, SellingToSomeoneOutOfMoney)
{
    Vendor vendor("Fishman", context);
    Character ch("Brian", context);

    // Load from the database
    ASSERT_TRUE(ch.loadFromDatabase(db));
    ASSERT_TRUE(vendor.loadFromDatabase(db));

    ch.inventory().lock()->pullMoney(object::Money{95});
    EXPECT_EQ(ch.inventory().lock()->money().convertToBaseMoney(), 5);

    EXPECT_FALSE(vendor.sell("object1", ch));
    EXPECT_EQ(ch.inventory().lock()->money().convertToBaseMoney(), 5);
}

/*
 * Trying to buy an object to a character when the vendor has not enough money
 */
TEST_F(VendorTest, BuyingWhenVendorIsOutOfMoney)
{
    Vendor vendor("Fishman", context);
    Character ch("Brian", context);

    // Load from the database
    ASSERT_TRUE(ch.loadFromDatabase(db));
    ASSERT_TRUE(vendor.loadFromDatabase(db));

    vendor.inventory().lock()->pullMoney(object::Money{9995});
    EXPECT_EQ(vendor.inventory().lock()->money().convertToBaseMoney(), 5);

    EXPECT_FALSE(vendor.buy("object1", ch));
    EXPECT_EQ(vendor.inventory().lock()->money().convertToBaseMoney(), 5);
}

/**
 * @brief Test setup (executed once)
 */
VendorTest::VendorTest()
{
    using ::testing::Return;
    config::ContextMock* mockContext = new config::ContextMock;
    EXPECT_CALL((*mockContext), kMapPath()).WillRepeatedly(Return("data"));
    context = std::shared_ptr<config::Context>(static_cast<config::Context*>(mockContext));

    object::Money::initialize("bronze",
                      std::pair<std::string, unsigned int>("argent", 100),
                      std::pair<std::string, unsigned int>("or", 50000));
    db = std::make_shared<database::Database>("data/sample1.sqlite");
}

} // namespace character
