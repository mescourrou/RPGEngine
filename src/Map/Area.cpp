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
            if (std::abs(pt1.y() - point.y()) > ZERO_EPSILON &&
                    std::abs(pt2.y() - point.y()) > ZERO_EPSILON)
                nbIntersections++;
            else if (std::abs(pt1.y() - point.y()) < ZERO_EPSILON)
            {
                if (std::signbit(pt1.y() - pt0.y()) != std::signbit(pt1.y() - pt2.y()))
                    nbIntersections++;
                else if (std::abs(pt1.x() - point.x()) < ZERO_EPSILON) // Same points
                    nbIntersections++;
            }
        }
    }
    return nbIntersections % 2 == 1;
}

/**
 * @brief Is the point q on the segment [p,r]
 * @param p 1st point of the segment
 * @param q Point to test
 * @param r 2nd point of the segment
 * @return True if q is on [p,r]
 */
bool Area::onSegment(const Vector<2>& p, const Vector<2>& q, const Vector<2>& r)
{
    // Use of https://www.geeksforgeeks.org/check-if-two-given-line-segments-intersect/

    if (q.x() <= std::max(p.x(), r.x()) && q.x() >= std::min(p.x(), r.x()) &&
        q.y() <= std::max(p.y(), r.y()) && q.y() >= std::min(p.y(), r.y()))
       return true;

    return false;
}

/**
 * @brief Find the orientation of the segments [p,q], [q,r] et [r,p]
 * @param p 1st point
 * @param q 2nd point
 * @param r 3rd point
 * @return 0 --> p, q and r are colinear
 * @return 1 --> Clockwise
 * @return 2 --> Counterclockwise
 */
int Area::orientation(const Vector<2>& p, const Vector<2>& q, const Vector<2>& r)
{
    // Use of https://www.geeksforgeeks.org/check-if-two-given-line-segments-intersect/

    // See https://www.geeksforgeeks.org/orientation-3-ordered-points/
    // for details of below formula.
    double val = (q.y() - p.y()) * (r.x() - q.x()) -
              (q.x() - p.x()) * (r.y( )- q.y());

    if (std::abs(val) < ZERO_EPSILON) return 0;  // colinear

    return (val > 0)? 1: 2; // clock or counterclock wise
}

/**
 * @brief Do segment [p1,q1] intersect [p2, q2]
 * @param p1 1st point of the 1st segment
 * @param q1 2nd point of the 1st segment
 * @param p2 1st point of the 2nd segment
 * @param q2 2nd point of the 2nd segment
 * @return True if the segments intersect
 */
bool Area::doIntersect(const Vector<2>& p1, const Vector<2>& q1, const Vector<2>& p2, const Vector<2>& q2)
{
    // Use of https://www.geeksforgeeks.org/check-if-two-given-line-segments-intersect/
    // Find the four orientations needed for general and special cases
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);

    // General case
    if (o1 != o2 && o3 != o4)
        return true;

    // Special Cases
    // p1, q1 and p2 are colinear and p2 lies on segment p1q1
    if (o1 == 0 && onSegment(p1, p2, q1)) return true;

    // p1, q1 and q2 are colinear and q2 lies on segment p1q1
    if (o2 == 0 && onSegment(p1, q2, q1)) return true;

    // p2, q2 and p1 are colinear and p1 lies on segment p2q2
    if (o3 == 0 && onSegment(p2, p1, q2)) return true;

     // p2, q2 and q1 are colinear and q1 lies on segment p2q2
    if (o4 == 0 && onSegment(p2, q1, q2)) return true;

    return false; // Doesn't fall in any of the above cases
}

/**
 * @brief Tells if the segment [origin, origin+vector] and calculate the intersection
 * @param origin Origin of the vector
 * @param vector Moving vector
 * @param [out] outIntersect Output of the intersection
 * @return True if there is an intersection
 */
bool Area::intersect(const Vector<2>& origin, const Vector<2> &vector, Vector<2>& outIntersect) const
{
    auto destination = origin+vector;

    for (unsigned int i = 0; i < m_points.size(); i++)
    {
        //const auto& pt0 = (i == 0) ? m_points.at(m_points.size()-1) : m_points.at(i-1);
        const auto& pt1 = m_points.at(i);
        const auto& pt2 = (i == m_points.size() - 1) ? m_points.at(0) : m_points.at(i+1);

        if (doIntersect(origin, destination, pt1, pt2))
        {
            // Use of https://www.geeksforgeeks.org/program-for-point-of-intersection-of-two-lines/
            double a1 = destination.y()- origin.y();
            double b1 = origin.x() - destination.x();
            double c1 = a1*(origin.x()) + b1*(origin.y());

            // Line CD represented as a2x + b2y = c2
            double a2 = pt2.y() - pt1.y();
            double b2 = pt1.x() - pt2.x();
            double c2 = a2*(pt1.x())+ b2*(pt2.y());

            double determinant = a1*b2 - a2*b1;

            if (std::abs(determinant) < ZERO_EPSILON)
            {
                // The lines are parallel.
                outIntersect = {-1,-1};
            }
            else
            {
                outIntersect.x() = (b2*c1 - b1*c2)/determinant;
                outIntersect.y() = (a1*c2 - a2*c1)/determinant;
            }
            return true;
        }

    }
    return false;
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
 * @brief Tells if the half line from the origin point intersect the segment given
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
        if (std::abs(lowPt.y() - highPt.y()) < ZERO_EPSILON)
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
        else if (intersect.x() <= origin.x())
            return true;
    }
    return false;
}

/**
 * @brief Tells if the half line from the origin point intersect the segment given
 * @param pt1 1st point of the segment
 * @param pt2 2nd point of the segment
 * @param origin Origin of the half line
 * @param dir Direction to take
 * @return Return true if there is an intersection
 */
bool Area::intersectXHalfLine(const Vector<2>& pt1, const Vector<2>& pt2, const Vector<2> &origin, Direction dir)
{
    const auto& leftPt = std::max(pt1, pt2, [](const Vector<2>& p1, const Vector<2>& p2){ return p1.x() < p2.x();});
    const auto& rightPt = std::min(pt1, pt2, [](const Vector<2>& p1, const Vector<2>& p2){ return p1.x() <= p2.x();});

    if (leftPt.x() >= origin.x() && rightPt.x() <= origin.x())
    {
        Vector<2> intersect{origin.x(), 0};
        if (std::abs(rightPt.x() - leftPt.x()) < ZERO_EPSILON)
            intersect.y() = pt1.y();
        else
        {
            double relativePosition = 1 - (leftPt.x() - origin.x()) / (leftPt.y() - rightPt.y());
            const auto& downyPt = std::max(pt1, pt2, [](const Vector<2>& p1, const Vector<2>& p2){ return p1.y() < p2.y();});
            const auto& hightyPt = std::min(pt1, pt2, [](const Vector<2>& p1, const Vector<2>& p2){ return p1.y() <= p2.y();});
            intersect.y() = hightyPt.y() + relativePosition * (downyPt.y() - hightyPt.y());
        }


        if (dir == DOWN && intersect.y() >= origin.y())
            return true;
        else if (intersect.y() <= origin.y())
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

