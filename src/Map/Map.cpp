#include "Map.hpp"

// Stl
#include <fstream>

// Project
#include <VerbosityLevels.hpp>
#include <Area.hpp>
#include <Tools.hpp>
#include <ConfigFiles.hpp>

// External lib
#include <glog/logging.h>
#include <json.hpp>

namespace map
{

/**
 * @brief Constructor of the map
 * @param context Context to use
 * @param name Name of the map. Must match the database
 */
Map::Map(std::shared_ptr<config::Context> context, const std::string& name) :
    m_name(name), m_context(context)
{
    VLOG(verbosityLevel::OBJECT_CREATION) << "Creating " << className() << " => " <<
                                          this;

}

/**
 * @brief Load the map from the json file named like the map
 *
 * We use Tiled to generate the files : https://www.mapeditor.org/
 * @return Return true if all went well
 */
bool Map::load()
{
    std::ifstream file(m_context->kMapPath() + "/" + Tools::snakeCase(
                           m_name) + ".json");
    if (file.is_open())
    {
        json json;
        file >> json;
        if (!json.is_object())
            return false;

        namespace mapFile = config::structure::mapFile;

        if (!json[mapFile::KEY_LAYERS].is_array())
            return false;

        for (auto layer : json[mapFile::KEY_LAYERS])
        {
            if (!layer[mapFile::KEY_LAYER_NAME].is_string())
                return false;
            if (layer[mapFile::KEY_LAYER_NAME] == mapFile::NAME_COLLISIONS_LAYER)
            {
                if (!loadCollisionLayer(layer))
                    return false;
                LOG(INFO) << "Loading collision layers successfully";
            }
            else if (layer[mapFile::KEY_LAYER_NAME] == mapFile::NAME_TELEPORTS_LAYER)
            {
                std::cout << "Found teleports" << std::endl;
            }
        }

    }
    else
    {
        LOG(ERROR) << "Impossible to open " << m_context->kMapPath() + "/" +
                   Tools::snakeCase(m_name) + ".json";
        return false;
    }
    return true;
}

/**
 * @brief Add a collision area to the collision layer
 * @param area Area to add
 */
void Map::addCollisionArea(const map::Area& area)
{
    m_collisionLayer.push_back(area);
}

/**
 * @brief Tells if the point is in a collision area
 * @param point Point to test
 * @return Return true if the point is in a collision area
 */
bool Map::collision(const Vector<2>& point) const
{
    for (const auto& area : m_collisionLayer)
    {
        if (area.isInside(point))
            return true;
    }
    return false;
}

/**
 * @brief Tells if there is a collision with the move vector and give the intersection (if there is)
 * @param origin Origin of the vector
 * @param moveVector Moving vector
 * @param [out] intersect Intersection point if there is one
 * @return Return true if there is an intersection and if the intersect parameter was modified
 */
bool Map::collision(const Vector<2>& origin, const Vector<2>& moveVector,
                    Vector<2>& intersect) const
{
    for (const auto& area : m_collisionLayer)
    {
        if (area.intersect(origin, moveVector, intersect))
            return true;
    }
    return false;
}

/**
 * @brief Add a teleport area
 * @param area Area of the teleport
 * @param destination Destination
 */
void Map::addTeleportArea(const Area& area, const Position& destination)
{

}

/**
 * @brief Tells if the point is in a teleport area
 * @param point Point to test
 * @param[out] destination Destination to teleport to
 * @return Return true if you are in a teleport area
 */
bool Map::doITeleport(const Vector<2>& point, Position& destination) const
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

/**
 * @brief Load the collision layer from the json object given
 * @param layer Json object containing the layer
 * @return Return true if all went well
 */
bool Map::loadCollisionLayer(const json& layer)
{
    if (!layer.is_object())
        return false;
    namespace mapFile = config::structure::mapFile;
    if (!layer[mapFile::KEY_OBJECTS].is_array())
        return false;
    for (auto& polygon : layer[mapFile::KEY_OBJECTS])
    {
        if (!polygon.is_object())
            return false;
        if (!polygon.contains(mapFile::KEY_VISIBLE)
                || !polygon[mapFile::KEY_VISIBLE].is_boolean())
            return false;
        // If the polygon is visible
        if (polygon[mapFile::KEY_VISIBLE].get<bool>())
        {
            if (!polygon.contains(mapFile::KEY_X) || !polygon.contains(mapFile::KEY_Y) ||
                    !polygon[mapFile::KEY_X].is_number() || !polygon[mapFile::KEY_Y].is_number())
                return false;
            Vector<2> polygonPosition;
            polygonPosition.x() = polygon[mapFile::KEY_X].get<double>();
            polygonPosition.y() = polygon[mapFile::KEY_Y].get<double>();

            if (!polygon.contains(mapFile::KEY_HEIGHT)
                    || !polygon.contains(mapFile::KEY_WIDTH) ||
                    !polygon[mapFile::KEY_HEIGHT].is_number()
                    || !polygon[mapFile::KEY_WIDTH].is_number())
                return false;

            // Polygon
            if (polygon[mapFile::KEY_HEIGHT].get<unsigned int>() == 0
                    && polygon[mapFile::KEY_WIDTH].get<unsigned int>() == 0)
            {
                if (!polygon[mapFile::KEY_POLYGON].is_array())
                {
                    return false;
                }
                Area a;
                for (auto& point : polygon[mapFile::KEY_POLYGON])
                {
                    if (!polygon.contains(mapFile::KEY_X) || !polygon.contains(mapFile::KEY_Y) ||
                            !polygon[mapFile::KEY_X].is_number() || !polygon[mapFile::KEY_Y].is_number())
                        return false;
                    a.addPoint(polygonPosition + Vector<2> {point[mapFile::KEY_X].get<double>(), point[mapFile::KEY_Y].get<double>()});
                }
                addCollisionArea(a);
            }
            else // Rectangle
            {
                unsigned int height = polygon[mapFile::KEY_HEIGHT].get<unsigned int>();
                unsigned int width = polygon[mapFile::KEY_WIDTH].get<unsigned int>();
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
