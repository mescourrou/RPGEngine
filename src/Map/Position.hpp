#pragma once

// Stl
#include <memory>
#include <iostream>

// Project
#include "general_config.hpp"
#include <BaseObject.hpp>
#include <BaseException.hpp>
#include <Vector.hpp>

#ifdef RPG_BUILD_TEST
#include <gtest/gtest.h>
#endif

namespace databaseTools
{
class Database;
}
namespace config
{
class Context;
}

namespace map
{
/**
 * @brief Manage position exception
 */
CREATE_EXCEPTION_CLASS(Position)


class Map;

#ifdef RPG_BUILD_TEST
class PositionTest;
#endif

/**
 * @brief Position on a map
 */
class Position : public BaseObject
{
    DECLARE_BASEOBJECT(Position)
#ifdef RPG_BUILD_TEST
    friend class map::PositionTest;
#endif
  public:
    /// @brief Default constructor
    Position() = default;
    Position(double x, double y, double z = 0);

    Position(const Position&) = default;
    Position(Position&&) = default;
    Position& operator=(const Position&) = default;
    Position& operator=(Position&&) = default;

    bool operator==(const Position& cmp) const;
    bool operator!=(const Position& cmp) const;

    /**
     * @brief Create a Position at origin of the map given
     * @param map Map to create the position with
     */
    explicit Position(std::shared_ptr<Map> map) : m_map(map) {}
    Position(std::shared_ptr<Map> map, double x, double y, double z = 0);
    /// @brief Destructor
    ~Position() override = default;

    bool loadFromDatabase(std::shared_ptr<databaseTools::Database> db,
                          std::shared_ptr<config::Context> context, const std::string& characterName);

    /// @brief Get the map
    std::shared_ptr<Map> map() const noexcept
    {
        return m_map;
    }
    /// @brief Change the map
    void setMap(std::shared_ptr<Map> map)
    {
        m_map = map;
    }

    ///@ brief Get and modify the x coordinate of the position
    double& x() noexcept
    {
        return m_position.x();
    }
    ///@ brief Get and modify the y coordinate of the position
    double& y() noexcept
    {
        return m_position.y();
    }
    ///@ brief Get and modify the z coordinate of the position
    double& z() noexcept
    {
        return m_position.z();
    }
    ///@ brief Get the x coordinate of the position
    double x() const noexcept
    {
        return m_position.x();
    }
    ///@ brief Get the x coordinate of the position
    double y() const noexcept
    {
        return m_position.y();
    }
    ///@ brief Get the x coordinate of the position
    double z()  const noexcept
    {
        return m_position.z();
    }

    void move(const Vector<3>& move);

    double distanceTo(const Position& other) const;
    /// @brief Compare the distance of the two positions
    double operator-(const Position& other)
    {
        return distanceTo(other);
    }

    /// @brief Move the position of the vector
    Position& operator+=(const Vector<3>& m)
    {
        move(m);
        return *this;
    }
    bool operator==(const Position& other);
    bool operator!=(const Position& other);

    static bool verifyDatabaseModel(std::shared_ptr<databaseTools::Database> db);
    static bool createDatabaseModel(std::shared_ptr<databaseTools::Database> db);
  private:

    std::shared_ptr<Map> m_map; ///< Map where we are on
    Vector<3> m_position; ///< Position vector
};

} // namespace map

std::ostream& operator<<(std::ostream& flux, const map::Position& pos);

