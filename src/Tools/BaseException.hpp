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

/**
 * @brief Base class for exceptions
 */
class BaseException : public std::exception
{
    static constexpr Counter m_errorCounter = Counter();
public:
    /**
     * @brief Create an enum-like error class to identificate errors
     */
    class Errors {
    private:
        unsigned int m_id; ///< Id of the error
    public:
        constexpr Errors(unsigned int id) noexcept : m_id(id) {}
        virtual ~Errors() = default;
        virtual bool operator==(const Errors& error) noexcept final { return error.m_id == m_id; }
        virtual bool operator!=(const Errors& error) noexcept final { return error.m_id != m_id; }
    };
    static const inline Errors UNKNOWN = Errors(__COUNTER__);
    static const inline Errors MUTEX = Errors(__COUNTER__);
    /**
     * @brief Construct an exception
     * @param w Description of the error
     * @param errorCode Error code (Errors)
     */
    BaseException(const std::string& w, const BaseException::Errors& errorCode = UNKNOWN) noexcept :
        m_what(w), m_code(errorCode) {}
    ~BaseException() override = default;

    /**
     * @brief Return the description of the exception
     */
    const char* what() const noexcept override { return m_what.c_str(); }
    /**
     * @brief Return the code of the error
     */
    virtual Errors code() const noexcept final { return m_code; }
protected:

    std::string m_what; ///< Description of the error
    Errors m_code; ///< Error code
};

