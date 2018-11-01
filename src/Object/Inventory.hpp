#pragma once

// Std lib
#include <algorithm>

// Project
#include "general_config.hpp"
#include <BaseObject.hpp>
#include <Object.hpp>

#ifdef RPG_BUILD_TEST
#include <gtest/gtest.h>
#endif

namespace object
{

#ifdef RPG_BUILD_TEST
class InventoryTest;
#endif

class Inventory : public BaseObject
{
#ifdef RPG_BUILD_TEST
    friend class object::InventoryTest;
    FRIEND_TEST(InventoryTest, Push);
#endif
public:
    Inventory() = default;
    ~Inventory() override = default;

    std::string className() const noexcept override { return "Inventory"; }

    std::shared_ptr<Object> get(unsigned int index) const;
    std::shared_ptr<Object> get(const std::string& objectName) const;
    std::shared_ptr<Object> pop(unsigned int index);
    std::shared_ptr<Object> pop(const std::string& objectName);
    void push(const std::shared_ptr<Object> &newObject);

    unsigned int size() const { return m_inventory.size(); }

private:
    std::list<std::shared_ptr<Object>> m_inventory;
};

} // namespace object
