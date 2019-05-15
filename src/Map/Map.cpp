#include "Map.hpp"

#include <glog/logging.h>
#include <VerbosityLevels.hpp>
#include <Area.hpp>

#include <json.hpp>
/**
 * @brief Constructor of the map
 * @param name Name of the map. Must match the database
 */
map::Map::Map(const std::string &name) : m_name(name)
{
    VLOG(verbosityLevel::OBJECT_CREATION) << "Creating " << className() << " => " << this;

}

void map::Map::addCollisionArea(const map::Area &area)
{
    m_collisionLayer.push_back(area);
}

bool map::Map::collision(const Vector<2> &point) const
{
    for (const auto& area : m_collisionLayer)
    {
        if (area.isInside(point))
            return true;
    }
    return false;
}

bool map::Map::doITeleport(const Vector<2> &point, Position &destination) const
{
    for (const auto& area : m_teleportArea)
    {
        if (area.first.isInside(point))
        {
            destination = area.second;
            return true;
        }
    }
    return false;
}

