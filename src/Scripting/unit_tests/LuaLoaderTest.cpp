#include "LuaLoaderTest.hpp"
#include <fstream>
#include <any>
#include <LuaManager.hpp>
#include <typeinfo>

class A
{
  public:
    A(const int val) : val_(val)
    {
    }

    int f()
    {
        std::cout << "f() : val=" << val_ << std::endl;
        return val_;
    }
  private:
    int val_;
    LUA_ADD_BINDING(A, f);

};

namespace scripting
{

class B
{
  public:
    B(const int val) : m_val(val)
    {

    }

    int getVal()
    {
        std::cout << "Returning val = " << m_val << std::endl;
        return m_val;
    }
  private:
    int m_val;
    LUA_ADD_BINDING(B, getVal);
};



TEST_F(LuaLoaderTest, Lua)
{


    // Call Lua function

    //        luabridge::LuaRef luafunc = luabridge::getGlobal(L, "luafunc");

    A a(90000);
    B b(919191);

    //        int ret = luafunc(&a);

    //        std::cout << ret << std::endl;

    //        luabridge::LuaRef p = luabridge::getGlobal(L, "p");
    //        p();


    LUA_EXPORT_OBJECT(a);
    LUA_EXPORT_OBJECT(b);

    LuaManager::getManager().runFile("data/test.lua");


}

}


