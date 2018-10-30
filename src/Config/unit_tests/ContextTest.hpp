#pragma once

#include <gtest/gtest.h>
#include <Context.hpp>

namespace config {

class ContextTest : public testing::Test
{
public:

    void SetUp() override;
    void TearDown() override;


    static char** m_argv;
};

}

