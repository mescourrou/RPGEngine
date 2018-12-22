#pragma once

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


};

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


