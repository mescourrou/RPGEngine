#pragma once

// I/O
#include <iostream>

// Standard library
#include <list>
#include <algorithm>
#include <string>

// Project
#include "general_config.hpp"

#ifdef RPG_BUILD_TEST
#include <gtest/gtest.h>
#endif

#ifdef RPG_BUILD_TEST
class BaseObjectTest;
#endif

class BaseObject
{
#ifdef RPG_BUILD_TEST
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
    BaseObject* m_parent;
    std::list<BaseObject*> m_children;
};


