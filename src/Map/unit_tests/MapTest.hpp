#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <Context.hpp>
#include <Map.hpp>

namespace map {

class MapTest : public testing::Test
{
public:
};

}

namespace config {
class ContextMock : public Context
{
public:
    ContextMock() {}
    MOCK_CONST_METHOD0(kMapPath, const std::string());
};

}

