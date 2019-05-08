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
#ifdef RPG_BUILD_TEST
    // Test class
    friend class BaseObjectTest;
    FRIEND_TEST(BaseObjectTest, SetParentAfter);
#endif

public:
    BaseObject(BaseObject* parent = nullptr) noexcept ;
    virtual ~BaseObject() noexcept ;

    virtual BaseObject *parent() const noexcept final;
    virtual std::string className() const noexcept = 0;
protected:
    virtual void setParent(BaseObject* parent) noexcept final;

private:
    void addChild(BaseObject* child) noexcept ;
    void removeChild(BaseObject* child) noexcept ;
    BaseObject* m_parent; ///< Parent of the object
    std::list<BaseObject*> m_children; ///< Children of the object
};


