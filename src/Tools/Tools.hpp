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

    static constexpr unsigned int FRONT_SIZE =
        sizeof("static constexpr char* Tools::GetTypeNameHelper<T>::getTypeName() [with T = ")
        - 1u;
    static constexpr unsigned int BACK_SIZE = sizeof("]") - 1u;

    template <typename T>
    struct GetTypeNameHelper
    {
        static constexpr char* getTypeName(void)
        {
            constexpr size_t size = sizeof(__PRETTY_FUNCTION__) - FRONT_SIZE - BACK_SIZE;
            char* name = new char[size];
            memcpy(name, __PRETTY_FUNCTION__ + FRONT_SIZE, size - 1u);
            return name;
        }
    };


};

/**
 * @brief Calculate a linear modulo for floats and linear at zero
 *
 * @param number Number to divise
 * @param div Divisor
 */
template<typename T>
T Tools::linearModulo(T number, T div)
{
    return (number / div - std::floor(number / div)) * div;
}

/**
 * @brief Convert some type into an other.
 *
 * Use strinstream >> and << operators
 */
template<typename T, typename U>
T Tools::convertTo(const U& a)
{
    std::stringstream ss;
    ss << a;
    T ret;
    ss >> ret;
    return ret;
}


