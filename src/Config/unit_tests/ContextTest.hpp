#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <Context.hpp>

namespace config
{
class Context;

class ContextTest : public testing::Test
{
  public:
    void SetUp() override;

    static char** m_argv;
    std::shared_ptr<Context> m_context;
};

class ContextMock : public Context
{
  public:
    ContextMock() {}
    MOCK_CONST_METHOD0(kMapPath, std::string());
};

}

