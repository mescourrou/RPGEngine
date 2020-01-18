#pragma once

#include <gtest/gtest.h>

namespace database
{
class Database;
}

namespace character
{

class CharacterTest : public testing::Test
{
  public:
    void SetUp() override;

    std::shared_ptr<database::Database> database;
};

}

