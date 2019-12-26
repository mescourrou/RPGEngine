#pragma once

namespace epstl
{

class exception
{
  public:
    explicit exception() noexcept = default;
    explicit exception(const char* what) noexcept : m_what(what) {}

    virtual const char* what() const noexcept final
    {
        return m_what;
    }
  private:
    const char* m_what = "";
};

class implementation_exception : public exception
{
  public:
    explicit implementation_exception() noexcept = default;
    explicit implementation_exception(const char* what) noexcept :
        exception(what) {}
};

}
