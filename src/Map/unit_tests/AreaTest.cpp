#include "AreaTest.hpp"

#include <Area.hpp>

namespace map {

TEST_F(AreaTest, InitializerList)
{
    Area a{{1,2},{3,4},{5,6}};

    ASSERT_EQ(a.pointCount(), 3);
    EXPECT_EQ(a.pointList().at(0), Vector<2>({1,2}));
    EXPECT_EQ(a.pointList().at(1), Vector<2>({3,4}));
    EXPECT_EQ(a.pointList().at(2), Vector<2>({5,6}));
}

TEST_F(AreaTest, InsertPointAtTheEnd)
{
    Area a{{1,2},{3,4}};
    a.addPoint({5,6});

    ASSERT_EQ(a.pointCount(), 3);
    EXPECT_EQ(a.pointList().at(0), Vector<2>({1,2}));
    EXPECT_EQ(a.pointList().at(1), Vector<2>({3,4}));
    EXPECT_EQ(a.pointList().at(2), Vector<2>({5,6}));

}

TEST_F(AreaTest, IntersectYHalfLine)
{
    {
        Vector<2> v1{0, 0};
        Vector<2> v2{2, 2};
        Vector<2> origin{0,1};

        EXPECT_TRUE(Area::intersectYHalfLine(v1, v2, origin));
    }
    {
        Vector<2> v1{1, 1};
        Vector<2> v2{2, 2};
        Vector<2> origin{0,1};

        EXPECT_TRUE(Area::intersectYHalfLine(v1, v2, origin));
    }
    {
        Vector<2> v1{0, 0};
        Vector<2> v2{2, 0};
        Vector<2> origin{0,1};

        EXPECT_FALSE(Area::intersectYHalfLine(v1, v2, origin));
    }
    {
        Vector<2> v1{1, 1};
        Vector<2> v2{2, 1};
        Vector<2> origin{0,1};

        EXPECT_TRUE(Area::intersectYHalfLine(v1, v2, origin));
    }
    {
        Vector<2> v1{1, 1};
        Vector<2> v2{2, 2};
        Vector<2> origin{1,1};

        EXPECT_TRUE(Area::intersectYHalfLine(v1, v2, origin));
    }
    {
        Vector<2> v1{1, 1};
        Vector<2> v2{2, 2};
        Vector<2> origin{2,1};

        EXPECT_FALSE(Area::intersectYHalfLine(v1, v2, origin));
    }
    {
        Vector<2> v1{1, 1};
        Vector<2> v2{2, 2};
        Vector<2> origin{2,1};

        EXPECT_TRUE(Area::intersectYHalfLine(v1, v2, origin, Area::LEFT));
    }

}

}
