#include "Map.hpp"

#include <fstream>

#include <glog/logging.h>
#include <VerbosityLevels.hpp>
#include <Area.hpp>

#include <json.hpp>

namespace map {

/**
 * @brief Constructor of the map
 * @param name Name of the map. Must match the database
 */
Map::Map(std::shared_ptr<config::Context> context, const std::string &name) :
    m_name(name), m_context(context)
{
    VLOG(verbosityLevel::OBJECT_CREATION) << "Creating " << className() << " => " << this;

}

bool Map::load(const std::string& filename)
{
    std::ifstream file(m_context->kMapPath() + "/" + filename + ".json");
    if (file.is_open())
    {
        m_json.clear();
        file >> m_json;
        if (!m_json.is_object())
            return false;

        if (!doLoadTilesets(m_json))
            return false;

        if (!m_json[KEY_LAYERS].is_array())
            return false;

        for (auto layer : m_json[KEY_LAYERS])
        {
            if (!layer[KEY_LAYER_NAME].is_string())
                return false;
            if (layer[KEY_LAYER_NAME] == NAME_COLLISIONS_LAYER)
            {
                if (!loadCollisionLayer(layer))
                    return false;
            }
            else if (layer[KEY_LAYER_NAME] == NAME_TELEPORTS_LAYER)
            {
                std::cout << "Found teleports" << std::endl;
            }
            else if (layer[KEY_LAYER_TYPE] == TYPE_DATA_LAYER)
            {
                if (!doLoadTiles(layer))
                    return false;
            }
        }

        return true;
    }
    else {
        LOG(ERROR) << "Impossible to open " << m_context->kMapPath() + "/" + filename + ".json";
        return false;
    }
}

void Map::addCollisionArea(const map::Area &area)
{
    m_collisionLayer.push_back(area);
}

bool Map::collision(const Vector<2> &point) const
{
    for (const auto& area : m_collisionLayer)
    {
        if (area.isInside(point))
            return true;
    }
    return false;
}

void Map::addTeleportArea(const Area &area, const Position &destination)
{

}

bool Map::doITeleport(const Vector<2> &point, Position &destination) const
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

bool Map::loadCollisionLayer(const json& layer)
{
    if (!layer.is_object())
        return false;
    if (!layer[KEY_OBJECTS].is_array())
        return false;
    for (auto& polygon : layer[KEY_OBJECTS])
    {
        if (!polygon.is_object())
            return false;
        if (!polygon.contains(KEY_VISIBLE) || !polygon[KEY_VISIBLE].is_boolean())
            return false;
        // If the polygon is visible
        if (polygon[KEY_VISIBLE].get<bool>())
        {
            if (!polygon.contains(KEY_X) || !polygon.contains(KEY_Y) ||
                    !polygon[KEY_X].is_number() || !polygon[KEY_Y].is_number())
                return false;
            Vector<2> polygonPosition;
            polygonPosition.x() = polygon[KEY_X].get<double>();
            polygonPosition.y() = polygon[KEY_Y].get<double>();

            if (!polygon.contains(KEY_HEIGHT) || !polygon.contains(KEY_WIDTH) || !polygon[KEY_HEIGHT].is_number() || !polygon[KEY_WIDTH].is_number())
                return false;

            // Polygon
            if (polygon[KEY_HEIGHT].get<unsigned int>() == 0 && polygon[KEY_WIDTH].get<unsigned int>() == 0)
            {
                if (!polygon[KEY_POLYGON].is_array())
                {
                    return false;
                }
                Area a;
                for (auto& point : polygon[KEY_POLYGON])
                {
                    if (!polygon.contains(KEY_X) || !polygon.contains(KEY_Y) ||
                            !polygon[KEY_X].is_number() || !polygon[KEY_Y].is_number())
                        return false;
                    a.addPoint(polygonPosition + Vector<2>{point[KEY_X].get<double>(), point[KEY_Y].get<double>()});
                }
                addCollisionArea(a);
            }
            else // Rectangle
            {
                unsigned int height = polygon[KEY_HEIGHT].get<unsigned int>();
                unsigned int width = polygon[KEY_WIDTH].get<unsigned int>();
                Area a{polygonPosition,
                       {polygonPosition.x(), polygonPosition.y() + height},
                       {polygonPosition.x() + width, polygonPosition.y() + height},
                       {polygonPosition.x() + width, polygonPosition.y()}
                      };
                addCollisionArea(a);

            }
        }
    }

    return true;
}

}
