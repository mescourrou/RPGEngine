#include "Area.hpp"

namespace map {

/**
 * @brief Construct an non empty area
 * @param list List of points
 */
Area::Area(std::initializer_list<Vector<2>> list) : m_points{list}
{

}

/**
 * @brief Add a point at the asked index
 *
 * @warning The use of a vector means that add a point in the middle of the polygon need to
 * move all the next points
 * @param point Point to add
 * @param index Index where adding the point
 */
void Area::addPoint(const Vector<2> &point, int index)
{
    if (std::abs(index) >= m_points.size() && index != -1)
        return;
    if (index < 0)
    {
        index = m_points.size() + index + 1;
    }
    auto it = m_points.begin();
    for (unsigned int i = 0; i < index ; i++)
    {
        ++it;
    }
    m_points.insert(it, point);
}

/**
 * @brief Tells if the point is inside the area
 * @param point Point to test
 * @return Return true if the point is inside the area
 */
bool Area::isInside(const Vector<2> &point) const
{
    unsigned int nbIntersections = 0;

    for (unsigned int i = 0; i < m_points.size(); i++)
    {
        const auto& pt0 = (i == 0) ? m_points.at(m_points.size()-1) : m_points.at(i-1);
        const auto& pt1 = m_points.at(i);
        const auto& pt2 = (i == m_points.size() - 1) ? m_points.at(0) : m_points.at(i+1);

        if (intersectYHalfLine(pt1, pt2, point))
        {
            if (pt1.y() != point.y() && pt2.y() != point.y())
                nbIntersections++;
            else if (pt1.y() == point.y())
            {
                if (std::signbit(pt1.y() - pt0.y()) != std::signbit(pt1.y() - pt2.y()))
                    nbIntersections++;
                else if (pt1.x() == point.x()) // Same points
                    nbIntersections++;
            }
        }
    }
    return nbIntersections % 2 == 1;
}

/**
 * @brief Move the area following the vector
 * @param vector Vector to follow
 */
void Area::move(const Vector<2> &vector)
{
    for (auto& pt : m_points)
    {
        pt += vector;
    }
}

/**
 * @brief Get the point list
 */
std::vector<Vector<2>> Area::pointList() const
{
    return m_points;
}

/**
 * @brief Get the number of points
 */
size_t Area::pointCount() const
{
    return m_points.size();
}

/**
 * @brief Tells if the half line from the origin point intersect the segment givent
 * @param pt1 1st point of the segment
 * @param pt2 2nd point of the segment
 * @param origin Origin of the half line
 * @param dir Direction to take
 * @return Return true if there is an intersection
 */
bool Area::intersectYHalfLine(const Vector<2>& pt1, const Vector<2>& pt2, const Vector<2> &origin, Direction dir)
{
    const auto& highPt = std::max(pt1, pt2, [](const Vector<2>& p1, const Vector<2>& p2){ return p1.y() < p2.y();});
    const auto& lowPt = std::min(pt1, pt2, [](const Vector<2>& p1, const Vector<2>& p2){ return p1.y() <= p2.y();});

    if (highPt.y() >= origin.y() && lowPt.y() <= origin.y())
    {
        Vector<2> intersect{0, origin.y()};
        if (lowPt.y() == highPt.y())
            intersect.x() = pt1.x();
        else
        {
            double relativePosition = 1 - (highPt.y() - origin.y()) / (highPt.y() - lowPt.y());
            const auto& rightyPt = std::max(pt1, pt2, [](const Vector<2>& p1, const Vector<2>& p2){ return p1.x() < p2.x();});
            const auto& leftyPt = std::min(pt1, pt2, [](const Vector<2>& p1, const Vector<2>& p2){ return p1.x() <= p2.x();});
            intersect.x() = leftyPt.x() + relativePosition * (rightyPt.x() - leftyPt.x());
        }


        if (dir == RIGHT && intersect.x() >= origin.x())
            return true;
        if (dir == LEFT && intersect.x() <= origin.x())
            return true;
    }
    return false;
}

/**
 * @brief Print the Area in the stream as a list of points
 * @param stream Stream to write into
 * @param area Area to write
 * @return Return the modified stream
 */
std::ostream& operator<<(std::ostream& stream, const Area& area)
{
    stream << "{ ";
    for (auto pt = area.m_points.begin(); pt != area.m_points.end(); ++pt)
    {
        stream << *pt;
        if (pt != area.m_points.end())
            stream << ", ";
    }
    stream << "}";

    return stream;
}
} // namespace map

