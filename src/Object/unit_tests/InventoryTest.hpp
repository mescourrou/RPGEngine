#pragma once

#include <gtest/gtest.h>
#include <Inventory.hpp>

namespace object {

class InventoryTest : public testing::Test
{
public:
    void SetUp() override;

    std::shared_ptr<Inventory> inventory;
};

}

