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
    BaseObject(BaseObject* parent = nullptr);
    virtual ~BaseObject();

    virtual BaseObject *parent() const final;
    virtual std::string className() const = 0;

protected:
    virtual void setParent(BaseObject* parent) final;

private:
    void addChild(BaseObject* child);
    void removeChild(BaseObject* child);
    BaseObject* m_parent;
    std::list<BaseObject*> m_children;
};


