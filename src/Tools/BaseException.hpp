#pragma once

// Project
#include "general_config.hpp"
#include <BaseObject.hpp>
#include <Counter.hpp>

#ifdef RPG_BUILD_TEST
#include <gtest/gtest.h>
#endif

#ifdef RPG_BUILD_TEST
class BaseExceptionTest;
#endif

class BaseException : public std::exception
{
    static constexpr Counter m_errorCounter = Counter();
public:
    class Errors {
    private:
        unsigned int m_id;
    public:
        constexpr Errors(unsigned int id) noexcept : m_id(id) {}
        virtual ~Errors() = default;
        virtual bool operator==(const Errors& error) noexcept final { return error.m_id == m_id; }
        virtual bool operator!=(const Errors& error) noexcept final { return error.m_id != m_id; }
    };
    static const inline Errors UNKNOWN = Errors(__COUNTER__);
    BaseException(const std::string& w, const BaseException::Errors& errorCode = UNKNOWN) noexcept :
        m_what(w), m_code(errorCode) {}
    ~BaseException() override = default;

    const char* what() const noexcept override { return m_what.c_str(); }
    virtual Errors code() const noexcept final { return m_code; }
protected:

    std::string m_what;
    Errors m_code;
};

