#pragma once

// Stl
#include <cmath>

// Project
#include "general_config.hpp"

// External lib
#include <glog/logging.h>

#ifdef RPG_BUILD_TEST
#include <gtest/gtest.h>
#endif

#ifdef RPG_BUILD_TEST
class ToolsTest;
#endif

/**
 * @brief Static class with differents tools
 */
class Tools
{
#ifdef RPG_BUILD_TEST
    friend class ToolsTest;
#endif
public:
    Tools() = delete;
    ~Tools() = delete;

    template<typename T, typename U>
    static T convertTo(const U& a);

    static std::string snakeCase(std::string str);
    template<typename T>
    static T linearModulo(T number, T div);


};

template<typename T>
T Tools::linearModulo(T number, T div)
{
    return (number/ div - std::floor(number / div)) * div;
}

/**
 * @brief Convert some type into an other.
 *
 * Use strinstream >> and << operators
 */
template<typename T, typename U>
T Tools::convertTo(const U &a)
{
    std::stringstream ss;
    ss << a;
    T ret;
    ss >> ret;
    return ret;
}


