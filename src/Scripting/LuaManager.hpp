#pragma once
#pragma comment(lib, "lua52.lib")
#include <lua.hpp>
#include <LuaBridge/LuaBridge.h>

namespace scripting
{

class LuaManager
{
    static constexpr unsigned int FRONT_SIZE =
        sizeof("static constexpr char* scripting::LuaManager::GetTypeNameHelper<T>::getTypeName() [with T = ")
        - 1u;
    static constexpr unsigned int BACK_SIZE = sizeof("]") - 1u;

    template <typename T>
    struct GetTypeNameHelper
    {
        static constexpr char* getTypeName(void)
        {
            constexpr size_t size = sizeof(__PRETTY_FUNCTION__) - FRONT_SIZE - BACK_SIZE;
            char* name = new char[size];
            memcpy(name, __PRETTY_FUNCTION__ + FRONT_SIZE, size - 1u);
            return name;
        }
    };
  public:
    static LuaManager& getManager()
    {
        static LuaManager manager;
        return manager;
    }
    ~LuaManager();

    template<typename class_t, typename ...Args>
    void addClassMethod(int (class_t::* method) (Args...), const char* bindingName)
    {
        luabridge::getGlobalNamespace(m_luaState)
        .beginClass<class_t>(GetTypeNameHelper<class_t>::getTypeName())
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
            std::cerr << lua_tostring (m_luaState, -1) << std::endl;

        }
    }
  private:
    LuaManager();

    lua_State* m_luaState  = nullptr;
};

class LuaBinding
{
  public:
    template<typename class_t, typename ...Args>
    LuaBinding(int (class_t::* method) (Args...), const char* bindingName)
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
