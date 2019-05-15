#pragma once
// STL
#include <vector>
#include <map>

// Project
#include "general_config.hpp"
#include <BaseObject.hpp>
#include <BaseException.hpp>
#include <Vector.hpp>
#include <Position.hpp>
#include <Area.hpp>

#ifdef RPG_BUILD_TEST
#include <gtest/gtest.h>
#endif

/**
 * @namespace map
 * @brief Namespace grouping all map related classes
 */
namespace map
{

#ifdef RPG_BUILD_TEST
class MapTest;
#endif

CREATE_EXCEPTION_CLASS(Map)

/**
 * @brief Manage the core map
 */
class Map : public BaseObject
{
    DECLARE_BASEOBJECT(Map)
#ifdef RPG_BUILD_TEST
    friend class map::MapTest;
#endif
public:
    Map(const std::string& name);
    /// @brief Destructor
    ~Map() override = default;
    Map(const Map& copy) = default;
    Map(Map&& move) = default;

    Map& operator=(const Map& copy) = default;
    Map& operator=(Map&& move) = default;

    /// @brief Get the name of the map
    std::string name() const noexcept { return m_name; }

    bool operator==(const Map& other) const noexcept { return m_name == other.m_name; }
    bool operator!=(const Map& other) const noexcept { return m_name != other.m_name; }

    void addCollisionArea(const Area& area);
    bool collision(const Vector<2>& point) const;

    void addTeleportArea(const Area& area, const Position& destination);
    bool doITeleport(const Vector<2>& point, Position& destination) const;

private:
    std::string m_name; ///< Name of the map

    std::vector<Area> m_collisionLayer;
    // std::vector<NPC> m_npcLayer; ///< TODO: Add this when NPC created
    std::map<Area, Position> m_teleportArea;

};

} // namespace map

