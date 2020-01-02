#pragma once

#include <quadtree_region.hpp>

#include <map>
#include <vector>
#include <gtest/gtest.h>

namespace epstl {

class quadtreeRegionTest : public ::testing::TestWithParam<std::pair<int, int>>
{
public:
    void SetUp() override
    {
        tree = new quadtree_region<int>(GetParam().first, GetParam().second);

        x_min = -GetParam().first / 2;
        x_max = x_min + GetParam().first;

        y_min = -GetParam().second / 2;
        y_max = y_min + GetParam().second;

        points.resize(GetParam().first * GetParam().second * 0.2);
        std::generate(points.begin(), points.end(), [&](){
            std::pair<int, int> pt;
            do
            {
                pt.first = std::rand() % GetParam().first + x_min;
                pt.second = std::rand() % GetParam().second + y_min;
            } while (std::find(points.begin(), points.end(), pt) != points.end());
            return pt;
        });



        for (const auto& pt : points)
        {
            tree->set(pt.first, pt.second);
        }
    }

    void TearDown() override
    {
        delete tree;
        points.clear();
    }
protected:
    typedef quadtree_region<int> tree_type;
    tree_type* tree;
    std::vector<std::pair<int, int>> points;
    int x_min = 0;
    int x_max = 0;

    int y_min = 0;
    int y_max = 0;
};

} // namespace epstl

