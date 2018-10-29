#pragma once

#include <gtest/gtest.h>
#include <Money.hpp>

namespace object {

class MoneyTest : public testing::Test
{
public:

    void SetUp() override;
    void TearDown() override;

    void _createInvalidInitializeList();
};

}

