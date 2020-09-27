#pragma once

// Stl
#include <vector>

// Project
#include <Vector.hpp>

#ifdef RPG_BUILD_TEST
#include <gtest/gtest.h>
#endif

namespace map
{
class Area;
}

namespace map
{

#ifdef RPG_BUILD_TEST
class AreaTest;
#endif

/**
 * @brief Area tool class. Manage polygon based area
 */
class Area
{
#ifdef RPG_BUILD_TEST
    friend class map::AreaTest;
    FRIEND_TEST(AreaTest, IntersectYHalfLine);
#endif
    friend std::ostream& operator<<(std::ostream& stream, const Area& area);
  public:
    /// @brief Constructor
    Area() = default;
    Area(std::initializer_list<Vector<2>> list);

    void addPoint(const Vector<2>& point, int index = -1);
    bool isInside(const Vector<2>& point) const;
    bool intersect(const Vector<2>& origin, const Vector<2>& vector,
                   Vector<2>& outIntersect) const;

    void move(const Vector<2>& vector);
    std::vector<Vector<2>> pointList() const;
    size_t pointCount() const;

  protected:
    /**
     * @brief Direction to look for intersection
     */
    enum Direction
    {
        LEFT,
        RIGHT,
        UP,
        DOWN
    };
    static bool intersectYHalfLine(const Vector<2>& pt1, const Vector<2>& pt2,
                                   const Vector<2>& origin, Direction dir = RIGHT);
    static bool intersectXHalfLine(const Vector<2>& pt1, const Vector<2>& pt2,
                                   const Vector<2>& origin, Direction dir = DOWN);

  private:
    /// List of the area points
    std::vector<Vector<2>> m_points;

    static bool onSegment(const Vector<2>& p, const Vector<2>& q,
                          const Vector<2>& r);
    static int orientation(const Vector<2>& p, const Vector<2>& q,
                           const Vector<2>& r);
    static bool doIntersect(const Vector<2>& p1, const Vector<2>& q1,
                            const Vector<2>& p2, const Vector<2>& q2);
};

std::ostream& operator<<(std::ostream& stream, const Area& area);

} // namespace map


