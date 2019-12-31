#include "quadtreeRegionTest.hpp"
#include <quadtree_region.hpp>

namespace epstl {


using ::testing::Values;

TEST_P(quadtreeRegionTest, SetAPixel)
{
    quadtree_region<int> tree(GetParam().first, GetParam().second);

    int x_min = -GetParam().first / 2;
    int x_max = x_min + GetParam().first;

    int y_min = -GetParam().second / 2;
    int y_max = y_min + GetParam().second;


    std::vector<std::pair<int, int>> points(GetParam().first * GetParam().second * 0.2);
    std::generate(points.begin(), points.end(), [&](){
        std::pair<int, int> pt;
        pt.first = std::rand() % GetParam().first + x_min;
        pt.second = std::rand() % GetParam().second + y_min;
        return pt;
    });

    for (const auto& pt : points)
    {
        tree.set(pt.first, pt.second);
    }
    for (int row = y_min ; row < y_max; row++)
    {
        for (int col = x_min ; col < x_max; col++)
        {
            if (std::find(points.begin(), points.end(), std::pair<int, int>{col, row}) != points.end())
                EXPECT_TRUE(tree.at(col, row));
            else
                EXPECT_FALSE(tree.at(col, row));
        }
    }
    tree.print(std::cout);

}

static const std::vector<std::pair<int, int>> dimensionValues{{7, 7}, {8, 8}, {10, 4}, {12, 5}, {7, 11}};

INSTANTIATE_TEST_CASE_P(QuadtreeRegionDimensions,
                         quadtreeRegionTest,
                         ::testing::ValuesIn(dimensionValues),
                        [](const testing::TestParamInfo<std::pair<int, int>>& info){
                            std::stringstream ss;
                            ss << info.param.first << "x" << info.param.second;
                            return ss.str();
                        });


} // namespace epstl
