#pragma once

#include <gtest/gtest.h>

namespace object {
class Inventory;

class InventoryTest : public testing::Test
{
public:
    void SetUp() override;

    std::shared_ptr<Inventory> inventory;
};

}

