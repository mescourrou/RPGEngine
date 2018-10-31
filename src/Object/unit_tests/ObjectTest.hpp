#pragma once

#include <gtest/gtest.h>
#include <Object.hpp>
#include <glog/logging.h>

namespace object {

class ObjectTest : public testing::Test
{
public:
    void SetUp() override;
};

}

