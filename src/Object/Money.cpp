#include "Money.hpp"

#include <Database.hpp>
#include <Model.hpp>
#include <Query.hpp>
#include <Tools.hpp>

bool object::Money::m_initialized = false;

/**
 * @brief Construct a money amount
 */
object::Money::Money()
{
    if (!m_initialized)
    {
        LOG(ERROR) << "Money system must be initialized before using";
        throw std::string("Money system must be initialized before using");
    }
    m_values.reset(new std::vector<unsigned int>(m_moneyNames.size(), 0));
}

/**
 * @brief Create the amount of money given
 * @param values Initializer list. Go from the base money to the most valuable type of money
 */
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
    // Spread carry
    spread();
}

/**
 * @brief Initialize the money system from the database
 * @param [in] db Database to use
 * @return Return true if the initialization is done correctly
 */
bool object::Money::initializeFromDatabase(std::shared_ptr<database::Database> db)
{
    namespace Model = database::Model::Money;
    using namespace database;
    if (!db)
        throw MoneyException("No database given.", Database::DatabaseException::MISSING_DATABASE);
    if (!verifyDatabaseModel(db))
        throw MoneyException("The database model is not correct", Database::DatabaseException::BAD_MODEL);
    if (m_initialized)
    {
        m_moneyNames.clear();
        m_initialized = false;
    }
    auto result = db->query(Query::createQuery<Query::SELECT>(Model::TABLE, db).sort(Model::VALUE));

    if (result.size() <= 1)
        return false;
    if (std::atoi(result.at(1).at(Model::VALUE).c_str()) != 1)
        throw MoneyException("Invalid base value");
    m_moneyNames.push_back(std::pair<std::string, unsigned int>(result.at(1).at(Model::NAME), 1));
    for (unsigned int i = 2; i < result.size(); i++)
    {
        m_moneyNames.push_back(std::pair<std::string, unsigned int>(result.at(i).at(Model::NAME),
                                                                    std::atoi(result.at(i).at(Model::VALUE).c_str())));
    }
    LOG(INFO) << "Initializing Money system successfully";
    return true;
}

/**
 * @brief Get the value of the money wanted
 * @param moneyName Name of the money to get
 */
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

/**
 * @brief End of initialize chain with variadic parameters
 * @param value Money name to add
 */
void object::Money::initializeAdditionnalValues(const std::pair<std::string, unsigned int>& value)
{
    m_moneyNames.push_back(value);
    m_initialized = true;
}

bool object::Money::verifyDatabaseModel(std::shared_ptr<database::Database> db)
{
    namespace Model = database::Model::Money;
    using namespace database;
    if (!db->isTable(Model::TABLE))
        return false;
    auto columnList = db->columnList(Model::TABLE);

    unsigned short goodColumns = 0;
    for (auto& column : columnList)
    {
        if (column == Model::NAME)
            goodColumns++;
        else if (column == Model::VALUE)
            goodColumns++;
        else
            return false;
    }
    if (goodColumns != 2)
        return false;
    return true;
}

bool object::Money::createDatabaseModel(std::shared_ptr<database::Database> db)
{
    namespace Model = database::Model::Money;
    using namespace database;
    if (!db)
        throw MoneyException("No database given.", database::Database::DatabaseException::MISSING_DATABASE);

    db->query(Query::createQuery<Query::CREATE>(Model::TABLE, db).ifNotExists()
              .column(Model::NAME).constraint(Model::NAME, Query::PRIMARY_KEY)
              .column(Model::VALUE, DataType::INTEGER).constraint(Model::VALUE, Query::UNIQUE));

    return verifyDatabaseModel(db);
}

/**
 * @brief Spread carry over the money types
 */
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

/**
 * @brief Get the names of the money types
 */
std::vector<std::string> object::Money::moneyNames()
{
    std::vector<std::string> retList;
    for (auto& money : m_moneyNames)
    {
        retList.push_back(money.first);
    }
    return retList;
}

/**
 * @brief Get the value of the asked money type
 * @param moneyName Type of the money asked
 */
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

/**
 * @brief Convert the money into only the base value
 */
unsigned int object::Money::convertToBaseMoney() const
{
    unsigned int sum = 0;
    for (unsigned int i = 0; i < m_moneyNames.size(); i++)
    {
        sum += m_values->at(i) * m_moneyNames.at(i).second;
    }

    return sum;
}

/**
 * @brief Add the quantity given
 * @param moneyName Type of the money you want to add
 * @param quantity Amount of money to add
 */
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

/**
 * @brief Substract the quantity given
 * @param moneyName Type of the money you want to remove
 * @param quantity Amount of money to substract
 */
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

/**
 * @brief Print the money into the stream
 * @param stream
 * @param money Stream into we will write
 * @return Return the modified stream
 */
std::ostream &operator<<(std::ostream &stream, const object::Money &money)
{
    unsigned int i = 0;
    auto moneyNames = object::Money::moneyNames();
    for (const auto& moneyType : moneyNames)
    {
        stream << money.value(moneyType) << " " << moneyType;
        if (i + 1 < moneyNames.size())
            stream << ", ";

        i++;
    }
    return stream;
}
