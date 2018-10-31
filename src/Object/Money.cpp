#include "Money.hpp"
bool object::Money::m_initialized = false;

object::Money::Money()
{
    if (!m_initialized)
    {
        LOG(ERROR) << "Money system must be initialized before using";
        throw std::string("Money system must be initialized before using");
    }
    m_values.reset(new std::vector<unsigned int>(m_moneyNames.size(), 0));
}

object::Money::Money(std::initializer_list<unsigned int> values) : Money()
{
    if (values.size() > m_moneyNames.size())
        LOG(FATAL) << "Number of values must be at worse equal to the number of money";
    unsigned int i = 0;
    for (auto& value : values)
    {
        add(m_moneyNames.at(i).first, value);
        i++;
    }
    spread();
}


unsigned int object::Money::value(const std::string &moneyName) const
{
    unsigned int i = 0;
    for (auto& money : m_moneyNames)
    {
        if (money.first == moneyName)
            return m_values->at(i);
        i++;
    }
    LOG(WARNING) << "Money " << moneyName << " not found";
    return 0;
}

bool object::Money::operator==(const object::Money &other) const
{
    return convertToBaseMoney() == other.convertToBaseMoney();
}

bool object::Money::operator!=(const object::Money &other) const
{
    return convertToBaseMoney() != other.convertToBaseMoney();
}

bool object::Money::operator<=(const object::Money &other) const
{
    return convertToBaseMoney() <= other.convertToBaseMoney();
}

bool object::Money::operator>=(const object::Money &other) const
{
    return convertToBaseMoney() >= other.convertToBaseMoney();
}

bool object::Money::operator>(const object::Money &other) const
{
    return convertToBaseMoney() > other.convertToBaseMoney();
}

bool object::Money::operator<(const object::Money &other) const
{
    return convertToBaseMoney() < other.convertToBaseMoney();
}

object::Money object::Money::operator+(const object::Money &other) const
{
    return Money{convertToBaseMoney() + other.convertToBaseMoney()};
}

object::Money &object::Money::operator++(int)
{
    add(m_moneyNames.front().first, 1);
    return *this;
}

object::Money &object::Money::operator+=(const object::Money &other)
{
    add(m_moneyNames.front().first, other.convertToBaseMoney());
    return *this;
}

object::Money object::Money::operator+(unsigned int toAdd) const
{
    return Money{convertToBaseMoney() + toAdd};
}

object::Money &object::Money::operator+=(unsigned int toAdd)
{
    add(m_moneyNames.front().first, toAdd);
    return *this;
}

object::Money object::Money::operator-(const object::Money &other) const
{
    long sub = convertToBaseMoney() - other.convertToBaseMoney();
    if (sub < 0)
        throw std::string("Not enough money");
    return Money{static_cast<unsigned int>(sub)};
}

object::Money &object::Money::operator--(int)
{
    sub(m_moneyNames.front().first, 1);
    return *this;
}

object::Money &object::Money::operator-=(const object::Money &other)
{
    sub(m_moneyNames.front().first, other.convertToBaseMoney());
    return *this;
}

object::Money object::Money::operator-(unsigned int toAdd) const
{
    long sub = convertToBaseMoney() - toAdd;
    if (sub < 0)
        throw std::string("Not enough money");
    return Money{static_cast<unsigned int>(sub)};
}

object::Money &object::Money::operator-=(unsigned int toAdd)
{
    sub(m_moneyNames.front().first, toAdd);
    return *this;
}

void object::Money::initializeAdditionnalValues(const std::pair<std::string, unsigned int>& value)
{
    m_moneyNames.push_back(value);
    m_initialized = true;
}

void object::Money::spread()
{
    for(unsigned int i = 0; i < m_moneyNames.size() -1; i++)
    {
        if (m_values->at(i) >= m_moneyNames.at(i+1).second / m_moneyNames.at(i).second)
        {
            m_values->at(i+1) += static_cast<unsigned int>(m_values->at(i) / (m_moneyNames.at(i+1).second / m_moneyNames.at(i).second));
            m_values->at(i) = m_values->at(i) % (m_moneyNames.at(i+1).second / m_moneyNames.at(i).second);
        }
    }
}


std::shared_ptr<std::vector<std::string>> object::Money::moneyNames()
{
    std::shared_ptr<std::vector<std::string>> retList(new std::vector<std::string>());
    for (auto& money : m_moneyNames)
    {
        retList->push_back(money.first);
    }
    return retList;
}

unsigned int object::Money::moneyValue(const std::string &moneyName)
{
    for (auto& money : m_moneyNames)
    {
        if (money.first == moneyName)
            return money.second;
    }
    LOG(WARNING) << "Money " << moneyName << " not found";
    return 0;
}

unsigned int object::Money::convertToBaseMoney() const
{
    unsigned int sum = 0;
    for (unsigned int i = 0; i < m_moneyNames.size(); i++)
    {
        sum += m_values->at(i) * m_moneyNames.at(i).second;
    }

    return sum;
}

void object::Money::add(const std::string &moneyName, unsigned int quantity)
{
    unsigned int i = 0;
    for (auto& money : m_moneyNames)
    {
        if (money.first == moneyName)
        {
            m_values->at(i) += quantity;
            spread();

            return;
        }
        i++;
    }
    LOG(ERROR) << "Money " << moneyName << " not found";
    throw std::string("Money ") + moneyName + std::string(" not found");
}

bool object::Money::sub(const std::string &moneyName, unsigned int quantity)
{
    unsigned int i = 0;
    for (auto& money : m_moneyNames)
    {
        if (money.first == moneyName)
        {
            if (quantity * money.second > convertToBaseMoney())
            {
                LOG(INFO) << "Not enough money to substract " << quantity;
                return false;
            }
            unsigned int moneyGreaterGrade = 1;
            while(m_values->at(i) < quantity)
            {
                if (i + moneyGreaterGrade >= m_moneyNames.size())
                {
                    LOG(FATAL) << "Unexpected error : not enough money";
                }
                if (m_values->at(i + moneyGreaterGrade) != 0)
                {
                    m_values->at(i) += m_moneyNames.at(i + moneyGreaterGrade).second / m_moneyNames.at(i).second;
                    m_values->at(i + moneyGreaterGrade)--;
                }
                else
                    moneyGreaterGrade++;

            }
            m_values->at(i) -= quantity;
            spread();
            return true;
        }
        i++;
    }
    LOG(ERROR) << "Money " << moneyName << " not found";
    throw std::string("Money ") + moneyName + std::string(" not found");
}

std::ostream &operator<<(std::ostream &stream, const object::Money &money)
{
    unsigned int i = 0;
    auto moneyNames = *object::Money::moneyNames();
    for (const auto& moneyType : moneyNames)
    {
        stream << money.value(moneyType) << " " << moneyType;
        if (i + 1 < object::Money::moneyNames()->size())
            stream << ", ";

        i++;
    }
    return stream;
}
