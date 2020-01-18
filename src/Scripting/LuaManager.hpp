#pragma once
#pragma comment(lib, "lua52.lib")
#include <lua.hpp>
#include <LuaBridge/LuaBridge.h>
#include <BaseException.hpp>
#include <Tools.hpp>


namespace scripting
{
CREATE_EXCEPTION_CLASS(Lua,
                       ADD_EXCEPTION_CODE(RUNNING));

class LuaManager
{
  public:
    static LuaManager& getManager()
    {
        static LuaManager manager;
        return manager;
    }
    ~LuaManager();

    template<typename class_t, typename return_t, typename ...Args>
    void addClassMethod(return_t (class_t::* method) (Args...), const char* bindingName)
    {
        luabridge::getGlobalNamespace(m_luaState)
        .beginClass<class_t>(Tools::GetTypeNameHelper<class_t>::getTypeName())
        .addFunction(bindingName, method)
        .endClass();
    }

    template<typename class_t>
    void exportObject(class_t& obj, const char* bindingName)
    {
        luabridge::setGlobal(m_luaState, &obj, bindingName);
    }

    void runFile(const char* filename)
    {
        if (luaL_dofile(m_luaState, filename))
        {
            throw LuaException(lua_tostring (m_luaState, -1), LuaException::RUNNING);
        }
    }

    void runString(const char* str)
    {
        if (luaL_dostring(m_luaState, str))
        {
            throw LuaException(lua_tostring (m_luaState, -1), LuaException::RUNNING);
        }
    }
  private:
    LuaManager();

    lua_State* m_luaState  = nullptr;
};

class LuaBinding
{
  public:
    template<typename class_t, typename return_t, typename ...Args>
    LuaBinding(return_t (class_t::* method) (Args...), const char* bindingName)
    {
        LuaManager::getManager().addClassMethod(method, bindingName);
    }
};

} // namespace scripting


#define LUA_ADD_BINDING(class_name, method_name) \
    static inline ::scripting::LuaBinding _##class_name##_##method_name = \
            ::scripting::LuaBinding(&class_name::method_name, #method_name)

#define LUA_EXPORT_OBJECT_WITH_NAME(object, binding_name) \
    ::scripting::LuaManager::getManager().exportObject(object, #binding_name)

#define LUA_EXPORT_OBJECT(object) \
    LUA_EXPORT_OBJECT_WITH_NAME(object, object)
