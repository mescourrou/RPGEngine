#include "LuaManager.hpp"
#ifdef RPG_GENERATE_LUA_BINDINGS
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
#endif
