#pragma once

#include <gtest/gtest.h>

namespace database
{
class Database;
}

namespace config
{
class Context;
}

namespace character
{

class VendorTest : public ::testing::Test
{
  public:
    VendorTest();

    std::shared_ptr<database::Database> db;
    std::shared_ptr<config::Context> context;
};

} // namespace character
