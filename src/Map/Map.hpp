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

// External libs
#include <json.hpp>
using json = nlohmann::json;

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

    bool load(const std::string& filename);

    /// @brief Get the name of the map
    std::string name() const noexcept { return m_name; }

    bool operator==(const Map& other) const noexcept { return m_name == other.m_name; }
    bool operator!=(const Map& other) const noexcept { return m_name != other.m_name; }

    void addCollisionArea(const Area& area);
    bool collision(const Vector<2>& point) const;

    void addTeleportArea(const Area& area, const Position& destination);
    bool doITeleport(const Vector<2>& point, Position& destination) const;

protected:
    bool loadCollisionLayer(const json &layer);

private:

    std::string m_name; ///< Name of the map
    json m_json;

    std::vector<Area> m_collisionLayer;
    // std::vector<NPC> m_npcLayer; ///< TODO: Add this when NPC created
    std::map<Area, Position> m_teleportArea;

    static inline constexpr char KEY_LAYERS[] = "layers";
    static inline constexpr char KEY_LAYER_NAME[] = "name";
    static inline constexpr char NAME_COLLISIONS_LAYER[] = "collisions";
    static inline constexpr char NAME_TELEPORTS_LAYER[] = "teleports";
    static inline constexpr char KEY_OBJECTS[] = "objects";
    static inline constexpr char KEY_VISIBLE[] = "visible";
    static inline constexpr char KEY_X[] = "x";
    static inline constexpr char KEY_Y[] = "y";
    static inline constexpr char KEY_HEIGHT[] = "height";
    static inline constexpr char KEY_WIDTH[] = "width";
    static inline constexpr char KEY_POLYGON[] = "polygon";

};

} // namespace map

