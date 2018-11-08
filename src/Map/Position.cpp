#include "Position.hpp"

map::Position::Position(std::shared_ptr<map::Map> map, double x, double y, double z) :
    m_map(map), m_position{x,y,z}
{
}

map::Position::Position(double x, double y, double z) :
    m_position{x,y,z}
{
}

void map::Position::move(const Vector<3> &move)
{
    m_position += move;
}

double map::Position::distanceTo(const map::Position &other) const
{
    if (m_map && other.m_map)
    {
        if (m_map->name() != other.m_map->name())
            throw PositionException("Position not in the same map");
    }
    return Vector<3>(other.m_position - m_position).norm();
}

bool map::Position::operator==(const map::Position &other)
{
    bool same = true;
    if (m_map && other.m_map)
    {
        if (*m_map != *other.m_map)
            same = false;
    }
    return m_position == other.m_position && same;
}

bool map::Position::operator!=(const map::Position &other)
{
    bool diff = false;
    if (m_map && other.m_map)
    {
        if (*m_map != *other.m_map)
            diff = true;
    }
    return m_position != other.m_position || diff;
}
