#pragma once

#include <gtest/gtest.h>

namespace databaseTools
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

    std::shared_ptr<databaseTools::Database> db;
    std::shared_ptr<config::Context> context;
};

} // namespace character
