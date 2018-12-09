#pragma once

#include <gtest/gtest.h>
#include <Character.hpp>

namespace character {

class CharacterTest : public testing::Test
{
public:
    void SetUp() override;

    std::shared_ptr<database::Database> database;
};

}

