#pragma once

#include <map>
#include <gtest/gtest.h>

namespace epstl {

class quadtreeRegionTest : public ::testing::TestWithParam<std::pair<int, int>>
{
};

} // namespace epstl

