#include "AreaTest.hpp"

#include <Area.hpp>

namespace map
{

/*
 * Test if the initialization with a initializerList works
 */
TEST_F(AreaTest, InitializerList)
{
    Area a{{1, 2}, {3, 4}, {5, 6}};

    ASSERT_EQ(a.pointCount(), 3);
    EXPECT_EQ(a.pointList().at(0), Vector<2>({1, 2}));
    EXPECT_EQ(a.pointList().at(1), Vector<2>({3, 4}));
    EXPECT_EQ(a.pointList().at(2), Vector<2>({5, 6}));
}

/*
 * Test insertion of a point at the end
 */
TEST_F(AreaTest, InsertPointAtTheEnd)
{
    Area a{{1, 2}, {3, 4}};
    a.addPoint({5, 6});

    ASSERT_EQ(a.pointCount(), 3);
    EXPECT_EQ(a.pointList().at(0), Vector<2>({1, 2}));
    EXPECT_EQ(a.pointList().at(1), Vector<2>({3, 4}));
    EXPECT_EQ(a.pointList().at(2), Vector<2>({5, 6}));
}

/*
 * Insertion of a point when empty
 */
TEST_F(AreaTest, InsertPointWhenEmpty)
{
    Area a;
    a.addPoint({1, 2});
    a.addPoint({3, 4});
    a.addPoint({5, 6});

    ASSERT_EQ(a.pointCount(), 3);
    EXPECT_EQ(a.pointList().at(0), Vector<2>({1, 2}));
    EXPECT_EQ(a.pointList().at(1), Vector<2>({3, 4}));
    EXPECT_EQ(a.pointList().at(2), Vector<2>({5, 6}));
}

/*
 * Test the intersection algorithm
 */
TEST_F(AreaTest, IntersectYHalfLine)
{
    {
        Vector<2> v1{0, 0};
        Vector<2> v2{2, 2};
        Vector<2> origin{0, 1};

        EXPECT_TRUE(Area::intersectYHalfLine(v1, v2, origin));
    }
    {
        Vector<2> v1{1, 1};
        Vector<2> v2{2, 2};
        Vector<2> origin{0, 1};

        EXPECT_TRUE(Area::intersectYHalfLine(v1, v2, origin));
    }
    {
        Vector<2> v1{0, 0};
        Vector<2> v2{2, 0};
        Vector<2> origin{0, 1};

        EXPECT_FALSE(Area::intersectYHalfLine(v1, v2, origin));
    }
    {
        Vector<2> v1{1, 1};
        Vector<2> v2{2, 1};
        Vector<2> origin{0, 1};

        EXPECT_TRUE(Area::intersectYHalfLine(v1, v2, origin));
    }
    {
        Vector<2> v1{1, 1};
        Vector<2> v2{2, 2};
        Vector<2> origin{1, 1};

        EXPECT_TRUE(Area::intersectYHalfLine(v1, v2, origin));
    }
    {
        Vector<2> v1{1, 1};
        Vector<2> v2{2, 2};
        Vector<2> origin{2, 1};

        EXPECT_FALSE(Area::intersectYHalfLine(v1, v2, origin));
    }
    {
        Vector<2> v1{1, 1};
        Vector<2> v2{2, 2};
        Vector<2> origin{2, 1};

        EXPECT_TRUE(Area::intersectYHalfLine(v1, v2, origin, Area::LEFT));
    }

}

/*
 * Test Inside with a convex polygon
 */
TEST_F(AreaTest, IsInsideConvex)
{
    Area a{{0, 0}, {1, 1}, {1, 2}, {-1, 1}, {0, 1}};

    EXPECT_TRUE(a.isInside(Vector<2> {1, 1}));
    EXPECT_FALSE(a.isInside(Vector<2> {0.6, 0.5}));
    EXPECT_TRUE(a.isInside(Vector<2> {1, 2}));
    EXPECT_FALSE(a.isInside(Vector<2> {0, 2}));
    EXPECT_FALSE(a.isInside(Vector<2> {-2, 1}));
    EXPECT_TRUE(a.isInside(Vector<2> {-0.5, 1}));
    EXPECT_FALSE(a.isInside(Vector<2> {-0.5, 1.5}));
}

/*
 * Test Inside with a concave polygon
 */
TEST_F(AreaTest, IsInsideConcave)
{
    Area a{{0, 0}, {2, 0}, {2, 1.5}, {1, 0.5}, {0, 1.5}};

    EXPECT_TRUE(a.isInside(Vector<2> {0.5, 0.5}));
    EXPECT_TRUE(a.isInside(Vector<2> {1.5, 0.5}));
    EXPECT_FALSE(a.isInside(Vector<2> {1, 1}));
    EXPECT_FALSE(a.isInside(Vector<2> {-0.5, 0.5}));

}

TEST_F(AreaTest, Intersections)
{
    /*
     *     x   x
     *
     *         x
     */
    Area a{{0, 4}, {4, 4}, {4, 0}};

    Vector<2> intersection;
    Vector<2> expectation;

    EXPECT_FALSE(a.intersect({0, 2}, {1.5, 0}, intersection));

    expectation = {2, 2};
    EXPECT_TRUE(a.intersect({0, 2}, {2, 0}, intersection));
    EXPECT_EQ(intersection, expectation);

    EXPECT_TRUE(a.intersect({0, 2}, {2.5, 0}, intersection));
    EXPECT_EQ(intersection, expectation);

    expectation = {1, 3};
    EXPECT_TRUE(a.intersect({0, 2}, {2, 2}, intersection));
    EXPECT_EQ(intersection, expectation);

    expectation = {4, 4};
    EXPECT_TRUE(a.intersect({4, 5}, {0, -1}, intersection));
    EXPECT_EQ(intersection, expectation);


    EXPECT_FALSE(a.intersect({4, 5}, {0, 1}, intersection));
    EXPECT_FALSE(a.intersect({5, 4}, {1, 0}, intersection));
    EXPECT_FALSE(a.intersect({2, 0}, {-1, 0}, intersection));

    EXPECT_FALSE(a.intersect({3, -1}, {2, 0}, intersection));
    EXPECT_FALSE(a.intersect({5, 4}, {0, 2}, intersection));


    Area a2{{1, 0}, {1, 3}, {2, 3}, {2, 0}};

    expectation = {1, 1};
    EXPECT_TRUE(a2.intersect({0, 1}, {3, 0}, intersection));
    EXPECT_EQ(intersection, expectation);

}

}
