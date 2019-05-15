#include "Area.hpp"

namespace map {

Area::Area(std::initializer_list<Vector<2>> list) : m_points{list}
{

}

void Area::addPoint(const Vector<2> &point, int index)
{
    if (std::abs(index) >= m_points.size())
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

std::vector<Vector<2>> Area::pointList() const
{
    return m_points;
}

size_t Area::pointCount() const
{
    return m_points.size();
}

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

} // namespace map
