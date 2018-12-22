#pragma once

#include <gtest/gtest.h>

namespace object {

class MoneyTest : public testing::Test
{
public:

    void SetUp() override;
    void TearDown() override;

    void _createInvalidInitializeList();
};

}

