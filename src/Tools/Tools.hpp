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

class Tools
{
#ifdef RPG_BUILD_TEST
    friend class ToolsTest;
#endif
public:
    Tools() = delete;
    ~Tools() = delete;


};


