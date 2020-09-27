/**
 * @file BaseException.hpp
 * @brief Define the BaseException class and usefull macros
 */
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
    class Errors
    {
      private:
        unsigned int m_id; ///< Id of the error
      public:
        explicit constexpr Errors(unsigned int id) noexcept : m_id(id) {}
        virtual ~Errors() = default;
        bool operator==(const Errors& error) const noexcept
        {
            return error.m_id == m_id;
        }
        bool operator!=(const Errors& error) const noexcept
        {
            return error.m_id != m_id;
        }
    };
    static const inline Errors UNKNOWN = Errors(__COUNTER__);
    static const inline Errors MUTEX = Errors(__COUNTER__);
    static const inline Errors LOADING = Errors(__COUNTER__);
    static const inline Errors MISSING_DATABASE = Errors(__COUNTER__);
    static const inline Errors BAD_MODEL = Errors(__COUNTER__);
    /**
     * @brief Construct an exception
     * @param w Description of the error
     * @param errorCode Error code (Errors)
     */
    BaseException(const std::string& w,
                  const BaseException::Errors& errorCode = UNKNOWN) noexcept :
        m_what(w), m_code(errorCode) {}
    /// @brief Default destructor
    ~BaseException() override = default;

    /**
     * @brief Return the description of the exception
     */
    const char* what() const noexcept override
    {
        return m_what.c_str();
    }
    /**
     * @brief Return the code of the error
     */
    virtual Errors code() const noexcept final
    {
        return m_code;
    }

  private:
    /// Description of the error
    std::string m_what;
    /// Error code
    Errors m_code;
};

/**
 * @def ADD_EXCEPTION_CODE(NAME)
 * @brief Create a new Error code attribute for Exception classes
 *
 * @param NAME Name of the error code
 */
#define ADD_EXCEPTION_CODE(NAME)                                                                        \
    static const inline Errors NAME = Errors(__COUNTER__);                                              \

/**
 * @def CREATE_EXCEPTION_CLASS(NAME, ...)
 * @brief Create the exception class corresponding to the given class name.
 *
 * To add codes, just add the list of ADD_EXCEPTION_CODE macro after the name of the class.
 *
 * Exemple:
 * @code
 * // Simple exception class
 * CREATE_EXCEPTION_CLASS(MySimpleClass)
 *
 * // With exceptions codes:
 * CREATE_EXCEPTION_CLASS(MyComplexClass,
 *                        ADD_EXCEPTION_CODE(CODE1) \
 *                        ADD_EXCEPTION_CODE(CODE2))
 * @endcode
 *
 * The exemple above will be processed into :
 * @code
 * class MySimpleClassException : public BaseException
 * {
 * public:
 * MySimpleClassException(const std::string& w, const Errors& code = BaseException::UNKNOWN):
 *        BaseException(w, code) {}
 * ~MySimpleClassException() override = default;
 * };
 *
 *
 * class MyComplexClassException : public BaseException
 * {
 * public:
 * static const inline Errors CODE1 = Errors(__COUNTER__);
 * static const inline Errors CODE2 = Errors(__COUNTER__);
 * MyComplexClassException(const std::string& w, const Errors& code = BaseException::UNKNOWN):
 *        BaseException(w, code) {}
 * ~MyComplexClassException() override = default;
 * };
 * @endcode
 *
 * @param NAME Name of the class attached to the Exception class
 */
#define __CREATE_EXCEPTION_CLASS(NAME, ...)                                                                 \
    class NAME : public BaseException                                                                       \
    {                                                                                                       \
      public:                                                                                               \
        __VA_ARGS__                                                                                         \
        NAME(const std::string& w, const Errors& code = BaseException::UNKNOWN):                            \
            BaseException(w, code) {}                                                                       \
        ~NAME() override = default;                                                                         \
    };

#define CREATE_EXCEPTION_CLASS(NAME, ...) __CREATE_EXCEPTION_CLASS(NAME##Exception, __VA_ARGS__)
