#include "LuaManager.hpp"

namespace scripting
{

LuaManager::~LuaManager()
{

}

LuaManager::LuaManager()
{
    m_luaState = luaL_newstate();
    luaL_openlibs(m_luaState);
}



} // namespace scripting

