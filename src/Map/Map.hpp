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
#include <Context.hpp>

// External libs
#ifdef RPG_BUILD_TEST
#include <gtest/gtest.h>
#endif
#include <json.hpp>
using json = nlohmann::json;


/**
 * @namespace map
 * @brief Namespace grouping all map related classes
 */
namespace map
{

#ifdef RPG_BUILD_TEST
class MapTest;
#endif

CREATE_EXCEPTION_CLASS(Map);

/**
 * @brief Manage the core map
 */
class Map : public BaseObject
{
    DECLARE_BASEOBJECT(Map);
#ifdef RPG_BUILD_TEST
    friend class map::MapTest;
#endif
  public:
    Map(std::shared_ptr<config::Context> context, const std::string& name);
    /// @brief Destructor
    ~Map() override = default;
    Map(const Map& copy) = default;
    Map(Map&& move) = default;

    Map& operator=(const Map& copy) = default;
    Map& operator=(Map&& move) = default;

    virtual bool load() final;

    /// @brief Get the name of the map
    virtual std::string name() const noexcept final
    {
        return m_name;
    }

    bool operator==(const Map& other) const noexcept
    {
        return m_name == other.m_name;
    }
    bool operator!=(const Map& other) const noexcept
    {
        return m_name != other.m_name;
    }

    virtual void addCollisionArea(const Area& area) final;
    virtual bool collision(const Vector<2>& point) const final;
    virtual bool collision(const Vector<2>& origin, const Vector<2>& moveVector,
                           Vector<2>& intersect) const final;

    virtual void addTeleportArea(const Area& area,
                                 const Position& destination) final;
    virtual bool doITeleport(const Vector<2>& point,
                             Position& destination) const final;

  protected:
    bool loadCollisionLayer(const json& layer);



  private:
    /// Name of the map
    std::string m_name;
    /// Context used
    std::shared_ptr<config::Context> m_context;

    /// List of the collisions areas
    std::vector<Area> m_collisionLayer;
    /// List of the teleports areas
    std::map<Area, Position> m_teleportArea;
};

} // namespace map

