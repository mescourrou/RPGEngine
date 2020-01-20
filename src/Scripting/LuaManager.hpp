#pragma once
#include <BaseException.hpp>
#include <Tools.hpp>
#include <functional>

#include <general_config.hpp>

#ifdef RPG_GENERATE_LUA_BINDINGS

#include <lua.hpp>
#include <LuaBridge/LuaBridge.h>

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

    template<typename class_t, typename first_t, typename ...Args>
    void addConstructor()
    {
        luabridge::getGlobalNamespace(m_luaState)
        .beginClass<class_t>(Tools::GetTypeNameHelper<class_t>::getTypeName())
        .template addConstructor<void (*) (first_t, Args...)>()
        .endClass();
    }

    template<typename class_t>
    void addConstructor()
    {
        luabridge::getGlobalNamespace(m_luaState)
        .beginClass<class_t>(Tools::GetTypeNameHelper<class_t>::getTypeName())
        .template addConstructor<void (*) (void)>()
        .endClass();
    }

    template<typename class_t, typename return_t, typename ...Args>
    void addClassMethod(return_t (class_t::* method) (Args...),
                        const char* bindingName)
    {
        luabridge::getGlobalNamespace(m_luaState)
        .beginClass<class_t>(Tools::GetTypeNameHelper<class_t>::getTypeName())
        .addFunction(bindingName, method)
        .endClass();
    }

    template<typename class_t, typename return_t, typename ...Args>
    void addClassMethod(return_t (class_t::* method) (Args...) const,
                        const char* bindingName)
    {
        luabridge::getGlobalNamespace(m_luaState)
        .beginClass<class_t>(Tools::GetTypeNameHelper<class_t>::getTypeName())
        .addFunction(bindingName, method)
        .endClass();
    }

    template<typename class_t, typename return_t, typename ...Args>
    void addClassStaticMethod(return_t (* method) (Args...),
                              const char* bindingName)
    {
        luabridge::getGlobalNamespace(m_luaState)
        .beginClass<class_t>(Tools::GetTypeNameHelper<class_t>::getTypeName())
        .addStaticFunction(bindingName, method)
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

template<typename class_t, typename ...Args>
class LuaConstructorBinding
{
  public:
    LuaConstructorBinding()
    {
        LuaManager::getManager().addConstructor<class_t, Args...>();
    }
};

template<typename class_t, typename return_t, typename ...Args>
class LuaBinding
{
  public:
    LuaBinding(return_t (class_t::* method) (Args...), const char* bindingName)
    {
        LuaManager::getManager().addClassMethod(method, bindingName);
    }

    LuaBinding(return_t (class_t::* method) (Args...) const,
               const char* bindingName)
    {
        LuaManager::getManager().addClassMethod(method, bindingName);
    }
};

template<typename class_t>
class LuaStaticBinding
{
  public:
    template<typename return_t, typename ...Args>
    LuaStaticBinding(return_t (method) (Args...), const char* bindingName)
    {
        LuaManager::getManager().addClassStaticMethod<class_t>(method, bindingName);
    }
};

} // namespace scripting

// -----------------------------------------------------------------------------------------
//                              CONSTRUCTOR BINDING
// -----------------------------------------------------------------------------------------
#define __GET_CONSTRUCTOR_NAME3(class_name, c) _##class_name##_constructor##_##c
#define __GET_CONSTRUCTOR_NAME2(class_name, c) __GET_CONSTRUCTOR_NAME3(class_name, c)
#define __GET_CONSTRUCTOR_NAME(class_name) __GET_CONSTRUCTOR_NAME2(class_name, __COUNTER__)
#define LUA_BIND_CONSTRUCTOR(class_name, ...) \
    static inline ::scripting::LuaConstructorBinding __GET_CONSTRUCTOR_NAME(class_name) = \
            ::scripting::LuaConstructorBinding<class_name, __VA_ARGS__>();

#define LUA_BIND_DEFAULT_CONSTRUCTOR(class_name) \
    static inline ::scripting::LuaConstructorBinding __GET_CONSTRUCTOR_NAME(class_name) = \
            ::scripting::LuaConstructorBinding<class_name>();

// -----------------------------------------------------------------------------------------
//                              METHOD BINDING
// -----------------------------------------------------------------------------------------
#define LUA_ADD_BINDING_WITH_NAME(class_name, method_name, binding_name) \
    static inline ::scripting::LuaBinding _##class_name##_##binding_name = \
            ::scripting::LuaBinding(&class_name::method_name, #binding_name);

#define LUA_ADD_BINDING(class_name, method_name) \
    LUA_ADD_BINDING_WITH_NAME(class_name, method_name, method_name)

#define LUA_ADD_BINDING_FOR_OVERLOAD(class_name, method_name, binding_name, return_type, ...) \
    static inline ::scripting::LuaBinding _##class_name##_##binding_name = \
            ::scripting::LuaBinding<class_name, return_type, __VA_ARGS__>(&class_name::method_name, #binding_name);

// -----------------------------------------------------------------------------------------
//                           STATIC METHOD BINDING
// -----------------------------------------------------------------------------------------

#define LUA_ADD_STATIC_BINDING_WITH_NAME(class_name, method_name, binding_name) \
    static inline ::scripting::LuaStaticBinding<class_name> _##class_name##_##binding_name = \
            ::scripting::LuaStaticBinding<class_name>(&class_name::method_name, #binding_name);

#define LUA_ADD_STATIC_BINDING(class_name, method_name) \
    LUA_ADD_STATIC_BINDING_WITH_NAME(class_name, method_name, method_name)

#define LUA_ADD_STATIC_BINDING_FOR_OVERLOAD(class_name, method_name, binding_name, return_type, ...) \
    static inline ::scripting::LuaBinding _##class_name##_##binding_name = \
            ::scripting::LuaStaticBinding<class_name, return_type, __VA_ARGS__>(&class_name::method_name, #binding_name);

// -----------------------------------------------------------------------------------------
//                              OBJECT EXPORT
// -----------------------------------------------------------------------------------------

#define LUA_EXPORT_OBJECT_WITH_NAME(object, binding_name) \
    ::scripting::LuaManager::getManager().exportObject(object, #binding_name);

#define LUA_EXPORT_OBJECT(object) \
    LUA_EXPORT_OBJECT_WITH_NAME(object, object)

#else
#define LUA_BIND_CONSTRUCTOR(class_name, ...)

#define LUA_ADD_BINDING(class_name, method_name)
#define LUA_ADD_BINDING_WITH_NAME(class_name, method_name, binding_name)
#define LUA_ADD_BINDING_FOR_OVERLOAD(class_name, method_name, binding_name, return_type, ...)

#define LUA_ADD_STATIC_BINDING_WITH_NAME(class_name, method_name, binding_name)
#define LUA_ADD_STATIC_BINDING(class_name, method_name)
#define LUA_ADD_STATIC_BINDING_FOR_OVERLOAD(class_name, method_name, binding_name, return_type, ...)

#define LUA_EXPORT_OBJECT_WITH_NAME(object, binding_name)
#define LUA_EXPORT_OBJECT(object)

#endif
