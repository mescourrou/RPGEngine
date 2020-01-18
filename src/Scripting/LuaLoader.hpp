#pragma once

// Project
#include "general_config.hpp"
#include <BaseObject.hpp>

#ifdef RPG_BUILD_TEST
#include <gtest/gtest.h>
#endif

namespace scripting
{

#ifdef RPG_BUILD_TEST
class LuaLoaderTest;
#endif

class LuaLoader : public BaseObject
{
    DECLARE_BASEOBJECT(LuaLoader)
#ifdef RPG_BUILD_TEST
    friend class scripting::LuaLoaderTest;
#endif
  public:
    LuaLoader();
    ~LuaLoader() override = default;
};

} // namespace scripting

