#pragma once

// I/O
#include <iostream>

// Stdlib
#include <memory>

// Project
#include "general_config.hpp"
#include <BaseObject.hpp>
#include <BaseException.hpp>

// External libs
#include <glog/logging.h>

#ifdef RPG_BUILD_TEST
#include <gtest/gtest.h>
#endif

namespace database {
class Database;
}

namespace object
{

class MoneyException : public BaseException
{
public:
    MoneyException(const std::string& w, const Errors& code = BaseException::UNKNOWN) noexcept : BaseException(w, code) {}
};

#ifdef RPG_BUILD_TEST
class MoneyTest;
#endif

/**
 * @brief Money management
 */
class Money : public BaseObject
{
#ifdef RPG_BUILD_TEST
    friend class object::MoneyTest;
#endif
public:
    Money();
    Money(std::initializer_list<unsigned int> values);
    /// @brief Destructor
    ~Money() override = default;
    Money(const Money& copy) = default;
    Money(Money&& move) = default;
    Money& operator=(const Money& copy) = default;
    Money &operator=(Money&& move) = default;

    template<typename ...Args>
    static void initialize(const std::string& baseValueName, Args...);
    static bool initializeFromDatabase(std::shared_ptr<database::Database> db);

    // --------- Getters ----------------
    // Static
    static std::vector<std::string> moneyNames();
    static unsigned int moneyValue(const std::string& moneyName);
    /**
     * @brief Return the number of money types
     */
    static unsigned long numberOfMoney() { return m_moneyNames.size(); }

    // Non static
    std::string className() const noexcept override { return "Money"; }
    unsigned int value(const std::string& moneyName) const;


    // --------- Operators ------------
    bool operator==(const Money& other) const;
    bool operator!=(const Money& other) const;
    bool operator<=(const Money& other) const;
    bool operator>=(const Money& other) const;
    bool operator>(const Money& other) const;
    bool operator<(const Money& other) const;

    Money operator+(const Money& other) const;
    Money& operator++(int);
    Money& operator+=(const Money& other);
    Money operator+(unsigned int toAdd) const;
    Money& operator+=(unsigned int toAdd);

    Money operator-(const Money& other) const;
    Money& operator--(int);
    Money& operator-=(const Money& other);
    Money operator-(unsigned int toAdd) const;
    Money& operator-=(unsigned int toAdd);

    unsigned int convertToBaseMoney() const;
    void add(const std::string& moneyName, unsigned int quantity);
    bool sub(const std::string& moneyName, unsigned int quantity);


    static bool verifyDatabaseModel(std::shared_ptr<database::Database> db);
    static bool createDatabaseModel(std::shared_ptr<database::Database> db);
protected:
    static bool m_initialized; ///< Say if the money system is initialized
    /// Contains the name of the moneys and their values from the base value
    static inline std::vector<std::pair<std::string, unsigned int>> m_moneyNames;

    template<typename ...Args>
    static void initializeAdditionnalValues(const std::pair<std::string, unsigned int> &value, Args... values);
    static void initializeAdditionnalValues(const std::pair<std::string, unsigned int> &value);


    void spread();

    std::shared_ptr<std::vector<unsigned int>> m_values; ///< Values of the instanced money


};

/** @fn void Money::initialize(const std::string &baseValueName, Args... values)
 * @brief Initialize the money system with the base value and the list of pairs <type name, type value>
 * @param baseValueName Name of the base value
 * @param values Values of the others money types
 */
template<typename ...Args>
void Money::initialize(const std::string &baseValueName, Args... values)
{
    m_moneyNames.clear();
    m_moneyNames.push_back(std::pair<std::string, unsigned int>(baseValueName, 1));
    initializeAdditionnalValues(values...);

    std::sort(m_moneyNames.begin(), m_moneyNames.end(), [](std::pair<std::string, unsigned int> a, std::pair<std::string, unsigned int> b)
    {
        return a.second < b.second;
    });

    LOG(INFO) << "Initializing Money system successfully";
}

/** @fn void Money::initializeAdditionnalValues(const std::pair<std::string, unsigned int>& value, Args... values)
 * @brief Add money types given
 * @param value Value to add : <type name, value in base value>
 * @param values Values of the others money types
 */
template<typename ...Args>
void Money::initializeAdditionnalValues(const std::pair<std::string, unsigned int>& value, Args... values)
{
    m_moneyNames.push_back(value);
    initializeAdditionnalValues(values...);
}

} // namespace object

std::ostream& operator<<(std::ostream& stream, const object::Money& money);

