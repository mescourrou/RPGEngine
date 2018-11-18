#pragma once

#include <gtest/gtest.h>
#include <Query.hpp>

namespace database {

class QueryTest : public testing::Test
{
public:
    void SetUp() override;

    std::shared_ptr<Database> database;
};

}

