#pragma once

#include <gtest/gtest.h>

namespace databaseTools
{
class Database;
}

namespace character
{

class CharacterTest : public testing::Test
{
  public:
    void SetUp() override;

    std::shared_ptr<databaseTools::Database> database;
};

}

