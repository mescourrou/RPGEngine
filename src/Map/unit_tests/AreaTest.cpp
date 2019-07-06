#include "AreaTest.hpp"

#include <Area.hpp>

namespace map {

/*
 * Test if the initialization with a initializerList works
 */
TEST_F(AreaTest, InitializerList)
{
    Area a{{1,2},{3,4},{5,6}};

    ASSERT_EQ(a.pointCount(), 3);
    EXPECT_EQ(a.pointList().at(0), Vector<2>({1,2}));
    EXPECT_EQ(a.pointList().at(1), Vector<2>({3,4}));
    EXPECT_EQ(a.pointList().at(2), Vector<2>({5,6}));
}

/*
 * Test insertion of a point at the end
 */
TEST_F(AreaTest, InsertPointAtTheEnd)
{
    Area a{{1,2},{3,4}};
    a.addPoint({5,6});

    ASSERT_EQ(a.pointCount(), 3);
    EXPECT_EQ(a.pointList().at(0), Vector<2>({1,2}));
    EXPECT_EQ(a.pointList().at(1), Vector<2>({3,4}));
    EXPECT_EQ(a.pointList().at(2), Vector<2>({5,6}));
}

/*
 * Insertion of a point when empty
 */
TEST_F(AreaTest, InsertPointWhenEmpty)
{
    Area a;
    a.addPoint({1,2});
    a.addPoint({3,4});
    a.addPoint({5,6});

    ASSERT_EQ(a.pointCount(), 3);
    EXPECT_EQ(a.pointList().at(0), Vector<2>({1,2}));
    EXPECT_EQ(a.pointList().at(1), Vector<2>({3,4}));
    EXPECT_EQ(a.pointList().at(2), Vector<2>({5,6}));
}

/*
 * Test the intersection algorithm
 */
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

/*
 * Test Inside with a convex polygon
 */
TEST_F(AreaTest, IsInsideConvex)
{
    Area a{{0,0}, {1,1}, {1,2}, {-1,1}, {0,1}};

    EXPECT_TRUE(a.isInside(Vector<2>{1,1}));
    EXPECT_FALSE(a.isInside(Vector<2>{0.6,0.5}));
    EXPECT_TRUE(a.isInside(Vector<2>{1,2}));
    EXPECT_FALSE(a.isInside(Vector<2>{0,2}));
    EXPECT_FALSE(a.isInside(Vector<2>{-2,1}));
    EXPECT_TRUE(a.isInside(Vector<2>{-0.5,1}));
    EXPECT_FALSE(a.isInside(Vector<2>{-0.5,1.5}));
}

/*
 * Test Inside with a concave polygon
 */
TEST_F(AreaTest, IsInsideConcave)
{
    Area a{{0,0}, {2,0},{2,1.5}, {1,0.5}, {0,1.5}};

    EXPECT_TRUE(a.isInside(Vector<2>{0.5,0.5}));
    EXPECT_TRUE(a.isInside(Vector<2>{1.5,0.5}));
    EXPECT_FALSE(a.isInside(Vector<2>{1,1}));
    EXPECT_FALSE(a.isInside(Vector<2>{-0.5,0.5}));

}

}
