#pragma once

// Project
#include "general_config.hpp"
#include <BaseObject.hpp>

#ifdef RPG_BUILD_TEST
#include <gtest/gtest.h>
#endif

namespace events
{

#ifdef RPG_BUILD_TEST
class WorkTest;
#endif

/**
 * @brief Base class for all templates implementation of Work
 */
class AbstractWork
{
#ifdef RPG_BUILD_TEST
	friend class events::WorkTest;
#endif
public:
    /// @brief Constructor
    AbstractWork() = default;
    /// @brief Destructor
    virtual ~AbstractWork() = default;

    /**
     * @brief Start the work
     */
    virtual void run() = 0;
};

/**
 * @class Work
 * @brief Template class for creating a work
 *
 * The template arguments take the type of the function arguments.
 *
 * Example:
 * @code
 * auto callback = [](std::string word, double number) {
 *     std::cout << word << " => " << number << std::endl;
 * };
 * Work<std::string, double> myWork(callback, "bla", 4.2);
 * myWork.run();
 * @endcode
 *
 * Output:
 * @code
 * bla => 4.2
 * @endcode
 */
template<typename ...Args>
class Work : public AbstractWork
{
public:
    /**
     * @brief Construct a work with the given function
     * @param func Function to do
     * @param arguments Arguments of the function
     */
    Work(std::function<void(Args...)> func, Args... arguments) : m_func(func), m_arguments(arguments...) {}
    ~Work() override = default;

    /**
     * @brief Run the job
     */
    void run() override
    {
        std::apply(m_func, m_arguments);
    }

private:
    std::function<void(Args...)> m_func; ///< Function to activate in run method
    std::tuple<Args...> m_arguments; ///< Arguments of the function


};

/**
 * @brief Specialization of Work to manage functions without arguments.
 */
template<>
class Work<void> : public AbstractWork
{
public:
    /**
     * @brief Construct a work with the given function
     * @param func Function to do
     */
    Work(std::function<void(void)> func) : m_func(func) {}
    ~Work() override = default;
    /**
     * @brief Run the job
     */
    void run() override
    {
        m_func();
    }

private:
    std::function<void(void)> m_func; ///< Function to activate in run method.
};

} // namespace events

