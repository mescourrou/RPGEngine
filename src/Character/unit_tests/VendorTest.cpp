#include "VendorTest.hpp"
#include <Vendor.hpp>
#include <Database.hpp>
#include <Object.hpp>
#include <unit_tests/ContextTest.hpp>

namespace character {

TEST_F(VendorTest, Buying)
{
    using ::testing::Return;
    config::ContextMock* context = new config::ContextMock;
    EXPECT_CALL((*context), kMapPath()).WillRepeatedly(Return("data"));
    auto contextPtr = std::shared_ptr<config::Context>(static_cast<config::Context*>(context));

    object::Money::initialize("bronze",
                      std::pair<std::string, unsigned int>("argent", 100),
                      std::pair<std::string, unsigned int>("or", 50000));
    Vendor vendor("Fishman", contextPtr);

    auto db = std::make_shared<database::Database>("data/sample1.sqlite");

    ASSERT_TRUE(vendor.loadFromDatabase(db));
    Character ch("Brian", contextPtr);

    // Load from the database
    ASSERT_TRUE(ch.loadFromDatabase(db));

    EXPECT_EQ(vendor.seeInventory().lock()->size(), 10);

    EXPECT_EQ(vendor.m_inventory->getNumberOf("object1"), 10);
    EXPECT_EQ(ch.m_inventory->getNumberOf("object1"), 1);
    EXPECT_EQ(ch.m_inventory->size(), 3);

    vendor.sell(0, ch);

    EXPECT_EQ(ch.m_inventory->size(), 4);
    EXPECT_EQ(vendor.m_inventory->getNumberOf("object1"), 9);
    EXPECT_EQ(ch.m_inventory->getNumberOf("object1"), 2);

    vendor.sell("object1", ch);

    EXPECT_EQ(ch.m_inventory->size(), 5);
    EXPECT_EQ(vendor.m_inventory->getNumberOf("object1"), 8);
    EXPECT_EQ(ch.m_inventory->getNumberOf("object1"), 3);
}

TEST_F(VendorTest, Selling)
{
    using ::testing::Return;
    config::ContextMock* context = new config::ContextMock;
    EXPECT_CALL((*context), kMapPath()).WillRepeatedly(Return("data"));
    auto contextPtr = std::shared_ptr<config::Context>(static_cast<config::Context*>(context));

    object::Money::initialize("bronze",
                      std::pair<std::string, unsigned int>("argent", 100),
                      std::pair<std::string, unsigned int>("or", 50000));
    Vendor vendor("Fishman", contextPtr);

    auto db = std::make_shared<database::Database>("data/sample1.sqlite");

    ASSERT_TRUE(vendor.loadFromDatabase(db));
    Character ch("Brian", contextPtr);

    // Load from the database
    ASSERT_TRUE(ch.loadFromDatabase(db));

    EXPECT_EQ(vendor.seeInventory().lock()->size(), 10);

    EXPECT_EQ(vendor.m_inventory->getNumberOf("object1"), 10);
    EXPECT_EQ(ch.m_inventory->getNumberOf("object1"), 1);
    EXPECT_EQ(ch.m_inventory->size(), 3);

    vendor.buy("object1", ch);

    EXPECT_EQ(ch.m_inventory->size(), 2);
    EXPECT_EQ(vendor.m_inventory->getNumberOf("object1"), 11);
    EXPECT_EQ(ch.m_inventory->getNumberOf("object1"), 0);

}

} // namespace character
