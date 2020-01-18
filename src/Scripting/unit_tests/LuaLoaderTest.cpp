#include "LuaLoaderTest.hpp"
#include <fstream>
#include <any>
#include <LuaManager.hpp>
#include <typeinfo>

class A : public BaseObject
{
    DECLARE_BASEOBJECT(A)
  public:

    A(const int val) : val_(val)
    {
    }

    int f()
    {
        std::cout << "f() : val=" << val_ << std::endl;
        return val_;
    }

    int g(int a)
    {
        std::cout << "value of a = " << a << std::endl;
        return a;
    }
  private:
    int val_;
    LUA_ADD_BINDING(A, f);

};

static A a(90000);

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

    void setValue(int value)
    {
        m_val = value;
    }

  private:
    int m_val;
    LUA_ADD_BINDING(B, getVal);
    LUA_ADD_BINDING(B, setValue);
};



TEST_F(LuaLoaderTest, LuaBinding)
{
    B b(919191);
    LUA_EXPORT_OBJECT(b);

    EXPECT_NO_THROW(LuaManager::getManager().runFile("data/test.lua"));
    EXPECT_THROW(LuaManager::getManager().runString("a:f()"), LuaException);


}

}


