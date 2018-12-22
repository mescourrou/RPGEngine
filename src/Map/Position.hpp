#pragma once

// Project
#include "general_config.hpp"
#include <BaseObject.hpp>
#include <BaseException.hpp>
#include <Vector.hpp>

#ifdef RPG_BUILD_TEST
#include <gtest/gtest.h>
#endif

namespace database {
class Database;
}

namespace map
{
class Map;

#ifdef RPG_BUILD_TEST
class PositionTest;
#endif

/**
 * @brief Position on a map
 */
class Position : public BaseObject
{
public:
    /**
     * @brief Manage position exception
     */
    class PositionException : BaseException
    {
    public:
        PositionException(const std::string& w, const Errors& code = BaseException::UNKNOWN) noexcept : BaseException(w, code) {}
    };
#ifdef RPG_BUILD_TEST
    friend class map::PositionTest;
#endif
public:
    /// @brief Default constructor
    Position() = default;
    Position(double x, double y, double z = 0);
    /**
     * @brief Create a Position at origin of the map given
     * @param map Map to create the position with
     */
    Position(std::shared_ptr<Map> map) : m_map(map) {}
    Position(std::shared_ptr<Map> map, double x, double y, double z = 0);
    /// @brief Destructor
    ~Position() override = default;

    bool loadFromDatabase(std::shared_ptr<database::Database> db, const std::string& characterName);

    /// @brief Get the map, but can't be modified
    const std::shared_ptr<Map> map() const noexcept { return m_map; }
    /// @brief Change the map
    void setMap(std::shared_ptr<Map> map) { m_map = map; }

    ///@ brief Get and modify the x coordinate of the position
    double& x() noexcept { return m_position.x(); }
    ///@ brief Get and modify the y coordinate of the position
    double& y() noexcept { return m_position.y(); }
    ///@ brief Get and modify the z coordinate of the position
    double& z() noexcept { return m_position.z(); }
    ///@ brief Get the x coordinate of the position
    double x() const noexcept { return m_position.x(); }
    ///@ brief Get the x coordinate of the position
    double y() const noexcept { return m_position.y(); }
    ///@ brief Get the x coordinate of the position
    double z()  const noexcept{ return m_position.z(); }

    void move(const Vector<3>& move);

    double distanceTo(const Position& other) const;
    /// @brief Compare the distance of the two positions
    double operator-(const Position& other) { return distanceTo(other); }

    /// @brief Move the position of the vector
    Position& operator+=(const Vector<3>& m) { move(m); return *this; }
    bool operator==(const Position& other);
    bool operator!=(const Position& other);

    std::string className() const noexcept override { return "Position"; }
private:
    static bool verifyDatabaseModel(std::shared_ptr<database::Database> db);

    std::shared_ptr<Map> m_map; ///< Map where we are on
    Vector<3> m_position; ///< Position vector
};

} // namespace map

