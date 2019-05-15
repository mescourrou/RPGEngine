#pragma once

#include <vector>

#include <Vector.hpp>

#ifdef RPG_BUILD_TEST
#include <gtest/gtest.h>
#endif

namespace map {

#ifdef RPG_BUILD_TEST
class AreaTest;
#endif

class Area
{
#ifdef RPG_BUILD_TEST
    friend class map::AreaTest;
    FRIEND_TEST(AreaTest, IntersectYHalfLine);
#endif
public:
    Area() = default;
    Area(std::initializer_list<Vector<2>> list);

    void addPoint(const Vector<2>& point, int index = -1);
    bool isInside(const Vector<2>& point) const;

    std::vector<Vector<2>> pointList() const;
    size_t pointCount() const;

protected:
    enum Direction {
        LEFT,
        RIGHT
    };
    static bool intersectYHalfLine(const Vector<2>& pt1, const Vector<2>& pt2, const Vector<2>& origin, Direction dir = RIGHT);
private:

    std::vector<Vector<2>> m_points;
};

} // namespace map
