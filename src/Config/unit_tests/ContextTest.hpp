#pragma once

#include <gtest/gtest.h>

namespace config {
class Context;

class ContextTest : public testing::Test
{
public:
    void SetUp() override;

    static char** m_argv;
    std::shared_ptr<Context> m_context;
};

}

