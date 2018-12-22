#pragma once

// Project
#include "general_config.hpp"
#include <BaseObject.hpp>

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

/**
 * @brief Manage the core map
 */
class Map : public BaseObject
{
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

    std::string className() const noexcept override { return "Map"; }

private:
    std::string m_name; ///< Name of the map
};

} // namespace map

