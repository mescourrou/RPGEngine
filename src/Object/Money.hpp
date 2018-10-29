#pragma once

// I/O
#include <iostream>

// Stdlib
#include <memory>

// Project
#include "general_config.hpp"
#include <BaseObject.hpp>

// External libs
#include <glog/logging.h>

#ifdef RPG_BUILD_TEST
#include <gtest/gtest.h>
#endif

namespace object
{

#ifdef RPG_BUILD_TEST
class MoneyTest;
#endif

class Money : public BaseObject
{
#ifdef RPG_BUILD_TEST
    friend class object::MoneyTest;
#endif
public:
    Money();
    Money(std::initializer_list<unsigned int> values);
    ~Money() override = default;
    Money(const Money& copy) = default;
    Money(Money&& move) = default;
    Money& operator=(const Money& copy) = default;
    Money &operator=(Money&& move) = default;

    template<typename ...Args>
    static void initialize(const std::string& baseValueName, Args...);

    // --------- Getters ----------------
    // Static
    static std::shared_ptr<std::vector<std::string> > moneyNames();
    static unsigned int moneyValue(const std::string& moneyName);

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


protected:
    static bool m_initialized;
    static inline std::vector<std::pair<std::string, unsigned int>> m_moneyNames;

    template<typename ...Args>
    static void initializeAdditionnalValues(const std::pair<std::string, unsigned int> &value, Args... values);
    static void initializeAdditionnalValues(const std::pair<std::string, unsigned int> &value);

    void spread();

    std::shared_ptr<std::vector<unsigned int>> m_values;


};

template<typename ...Args>
void Money::initialize(const std::string &baseValueName, Args... values)
{
    m_moneyNames.clear();
    m_moneyNames.push_back(std::pair<std::string, unsigned int>(baseValueName, 1));
    initializeAdditionnalValues(values...);

}

template<typename ...Args>
void Money::initializeAdditionnalValues(const std::pair<std::string, unsigned int>& value, Args... values)
{
    m_moneyNames.push_back(value);
    initializeAdditionnalValues(values...);
}

} // namespace object

std::ostream& operator<<(std::ostream& stream, const object::Money& money);

