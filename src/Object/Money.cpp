#include "Money.hpp"

// Project
#include <Database.hpp>
#include <Model.hpp>
#include <Query.hpp>
#include <Tools.hpp>
#include <VerbosityLevels.hpp>
#include <InstrumentationTimer.hpp>

namespace object
{

bool Money::m_initialized = false;

/**
 * @brief Construct a money amount
 */
Money::Money()
{
    PROFILE_FUNCTION();
    VLOG(verbosityLevel::OBJECT_CREATION) << "Creating " << className() << " => " <<
                                          this;
    if (!m_initialized)
    {
        LOG(ERROR) << "Money system must be initialized before using";
        throw MoneyException("Money system must be initialized before using",
                             MoneyException::INITIALIZATION);
    }
    m_values = std::make_shared<std::vector<unsigned int>>(m_moneyNames.size(), 0);
}

/**
 * @brief Create the amount of money given
 * @param values Initializer list. Go from the base money to the most valuable type of money
 */
Money::Money(std::initializer_list<unsigned int> values) : Money()
{
    PROFILE_FUNCTION();
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
bool Money::initializeFromDatabase(std::shared_ptr<databaseTools::Database> db)
{
    PROFILE_FUNCTION();
    namespace Model = database::Model::Money;
    using namespace databaseTools;
    if (!db)
        throw MoneyException("No database given.", BaseException::MISSING_DATABASE);
    if (!verifyDatabaseModel(db))
        throw MoneyException("The database model is not correct",
                             BaseException::BAD_MODEL);
    if (m_initialized)
    {
        m_moneyNames.clear();
        m_initialized = false;
    }
    auto result = db->query(Query::createQuery<Query::SELECT>(Model::TABLE,
                            db).sort(Model::VALUE));

    if (result.size() <= 1)
        return false;
    if (std::atoi(result.at(1).at(Model::VALUE).c_str()) != 1)
        throw MoneyException("Invalid base value");
    m_moneyNames.push_back(std::pair<std::string, unsigned int>(result.at(1).at(
                               Model::NAME), 1));
    for (unsigned int i = 2; i < result.size(); i++)
    {
        m_moneyNames.push_back(std::pair<std::string, unsigned int>(result.at(i).at(
                                   Model::NAME),
                               std::atoi(result.at(i).at(Model::VALUE).c_str())));
    }
    LOG(INFO) << "Initializing Money system successfully";
    m_initialized = true;
    return true;
}

/**
 * @brief Get the value of the money wanted
 * @param moneyName Name of the money to get
 */
unsigned int Money::value(const std::string& moneyName) const
{
    PROFILE_FUNCTION();
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

bool Money::operator==(const Money& other) const
{
    return convertToBaseMoney() == other.convertToBaseMoney();
}

bool Money::operator!=(const Money& other) const
{
    return convertToBaseMoney() != other.convertToBaseMoney();
}

bool Money::operator<=(const Money& other) const
{
    return convertToBaseMoney() <= other.convertToBaseMoney();
}

bool Money::operator>=(const Money& other) const
{
    return convertToBaseMoney() >= other.convertToBaseMoney();
}

bool Money::operator>(const Money& other) const
{
    return convertToBaseMoney() > other.convertToBaseMoney();
}

bool Money::operator<(const Money& other) const
{
    return convertToBaseMoney() < other.convertToBaseMoney();
}

Money Money::operator+(const Money& other) const
{
    return Money{convertToBaseMoney() + other.convertToBaseMoney()};
}

Money& Money::operator++(int)
{
    add(m_moneyNames.front().first, 1);
    return *this;
}

Money& Money::operator+=(const Money& other)
{
    add(m_moneyNames.front().first, other.convertToBaseMoney());
    return *this;
}

Money Money::operator+(unsigned int toAdd) const
{
    return Money{convertToBaseMoney() + toAdd};
}

Money& Money::operator+=(unsigned int toAdd)
{
    add(m_moneyNames.front().first, toAdd);
    return *this;
}

Money Money::operator-(const Money& other) const
{
    long sub = convertToBaseMoney() - other.convertToBaseMoney();
    if (sub < 0)
        throw MoneyException("Not enough money");
    return Money{static_cast<unsigned int>(sub)};
}

Money& Money::operator--(int)
{
    sub(m_moneyNames.front().first, 1);
    return *this;
}

Money& Money::operator-=(const Money& other)
{
    sub(m_moneyNames.front().first, other.convertToBaseMoney());
    return *this;
}

Money Money::operator-(unsigned int toAdd) const
{
    long sub = convertToBaseMoney() - toAdd;
    if (sub < 0)
        throw MoneyException("Not enough money");
    return Money{static_cast<unsigned int>(sub)};
}

Money& Money::operator-=(unsigned int toAdd)
{
    sub(m_moneyNames.front().first, toAdd);
    return *this;
}

/**
 * @brief End of initialize chain with variadic parameters
 * @param value Money name to add
 */
void Money::initializeAdditionnalValues(const
                                        std::pair<std::string, unsigned int>& value)
{
    m_moneyNames.push_back(value);
    m_initialized = true;
}

/**
 * @brief Verify the database model
 * @param db Database to verify
 * @return Return true if the database contains the good tables
 */
bool Money::verifyDatabaseModel(std::shared_ptr<databaseTools::Database> db)
{
    PROFILE_FUNCTION();
    namespace Model = database::Model::Money;
    using namespace databaseTools;
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

/**
 * @brief Create the table needed in the database
 * @param db Database to populate
 * @return Return true if the database was well populated
 */
bool Money::createDatabaseModel(std::shared_ptr<databaseTools::Database> db)
{
    PROFILE_FUNCTION();
    namespace Model = database::Model::Money;
    using namespace databaseTools;
    if (!db)
        throw MoneyException("No database given.", BaseException::MISSING_DATABASE);

    db->query(Query::createQuery<Query::CREATE>(Model::TABLE, db).ifNotExists()
              .column(Model::NAME).constraint(Model::NAME, Query::PRIMARY_KEY)
              .column(Model::VALUE, DataType::INTEGER).constraint(Model::VALUE,
                      Query::UNIQUE));

    return verifyDatabaseModel(db);
}

/**
 * @brief Spread carry over the money types
 */
void Money::spread()
{
    PROFILE_FUNCTION();
    for (unsigned int i = 0; i < m_moneyNames.size() - 1; i++)
    {
        if (m_values->at(i) >= m_moneyNames.at(i + 1).second / m_moneyNames.at(
                    i).second)
        {
            m_values->at(i + 1) += static_cast<unsigned int>(m_values->at(i) /
                                   (m_moneyNames.at(i + 1).second / m_moneyNames.at(i).second));
            m_values->at(i) = m_values->at(i) % (m_moneyNames.at(i + 1).second /
                                                 m_moneyNames.at(i).second);
        }
    }
}

/**
 * @brief Get the names of the money types
 */
std::vector<std::string> Money::moneyNames()
{
    PROFILE_FUNCTION();
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
unsigned int Money::moneyValue(const std::string& moneyName)
{
    PROFILE_FUNCTION();
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
unsigned int Money::convertToBaseMoney() const
{
    PROFILE_FUNCTION();
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
void Money::add(const std::string& moneyName, unsigned int quantity)
{
    PROFILE_FUNCTION();
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
    throw MoneyException(std::string("Money ") + moneyName +
                         std::string(" not found"));
}

/**
 * @brief Substract the quantity given
 * @param moneyName Type of the money you want to remove
 * @param quantity Amount of money to substract
 */
bool Money::sub(const std::string& moneyName, unsigned int quantity)
{
    PROFILE_FUNCTION();
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
            while (m_values->at(i) < quantity)
            {
                if (i + moneyGreaterGrade >= m_moneyNames.size())
                {
                    LOG(FATAL) << "Unexpected error : not enough money";
                }
                if (m_values->at(i + moneyGreaterGrade) != 0)
                {
                    m_values->at(i) += m_moneyNames.at(i + moneyGreaterGrade).second /
                                       m_moneyNames.at(i).second;
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
    throw MoneyException(std::string("Money ") + moneyName +
                         std::string(" not found"));
}

} // namespace object
/**
 * @brief Print the money into the stream
 * @param stream
 * @param money Stream into we will write
 * @return Return the modified stream
 */
std::ostream& operator<<(std::ostream& stream, const object::Money& money)
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

