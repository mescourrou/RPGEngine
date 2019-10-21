/**
 * @file BaseObject.hpp
 * @brief Declaration of the abstract BaseObject class and some usefull macros
 */
#pragma once

// I/O
#include <iostream>

// Standard library
#include <list>
#include <algorithm>
#include <string>
#include <string_view>

// Project
#include "general_config.hpp"

#ifdef RPG_BUILD_TEST
#include <gtest/gtest.h>
#endif

#ifdef RPG_BUILD_TEST
class BaseObjectTest;
#endif

/**
 * @def DECLARE_BASEOBJECT(name)
 * @brief Create the must have methods for every class derivated from BaseObject
 *
 * @param name Name of the class
 */
#define DECLARE_BASEOBJECT(name)                                                                    \
    public:                                                                                         \
    std::string className() const noexcept override { return #name; }                           \
    private:                                                                                        \


/**
 * @brief Common root object
 */
class BaseObject
{
  public:
    BaseObject() = default ;
    virtual ~BaseObject() = default;

    BaseObject(const BaseObject& copy) = default;
    BaseObject(BaseObject&& move) = default;

    virtual BaseObject& operator=(const BaseObject& copy) = default;
    virtual BaseObject& operator=(BaseObject&& move) = default;

    /**
     * @brief Get the class name
     */
    virtual std::string className() const noexcept = 0;
};


