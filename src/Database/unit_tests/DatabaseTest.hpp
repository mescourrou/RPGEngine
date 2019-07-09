#pragma once

#include <gtest/gtest.h>
#include <Database.hpp>

namespace database {

class DatabaseTest : public testing::Test
{
public:
    void SetUp() override;

    std::shared_ptr<Database> m_database;
};

}

