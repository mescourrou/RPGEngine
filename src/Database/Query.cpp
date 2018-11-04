#include "Query.hpp"

/**
 * @brief Verify if the column name is valid
 *
 * Current contraints:
 * - No spaces
 *
 * @param name
 * @return
 */
bool database::Query::checkColumnName(const std::string &name)
{
    if (name.find(' ') != std::string::npos)
    {
        m_valid = false;
        LOG(ERROR) << "Not valid column name : '" << name << "'";
        return false;
    }
    return true;
}

std::string database::SelectQuery::str() const
{
    std::stringstream ss;
    ss << "SELECT ";
    if (m_columns.size() == 0)
        ss << "*";
    else
    {
        for (unsigned int i = 0; i < m_columns.size(); i++)
        {
            ss << m_columns.at(i);
            if (i + 1 < m_columns.size())
                ss << ", ";
        }
    }
    ss << " FROM " << m_table;
    if (m_conditions.size() != 0)
    {
        ss << " WHERE ";
        for (unsigned int i = 0; i < m_conditions.size(); i++)
        {
            ss << m_conditions.at(i);
            if (i + 1 < m_conditions.size())
                ss << " AND ";
        }
    }

    ss << ";";
    return ss.str();
}


/**
 * @brief Add the couple <column, value> to the query
 * @param column Column of the value
 * @param value Value to insert
 * @return New query
 */
database::InsertQuery &database::InsertQuery::value(const std::string &column, const std::string &value)
{
    m_valid = true;
    if (!checkColumnName(column))
        return *this;
    m_values.push_back(std::pair<std::string, std::string>(column, value));
    return *this;
}

std::string database::InsertQuery::str() const
{
    if (!isValid())
        return {};
    std::stringstream ss;
    ss << "INSERT INTO ";
    ss << m_table;

    ss << " (";
    for (auto value : m_values)
    {
        ss << value.first;
        if (value != m_values.back())
            ss << ", ";
    }
    ss << ") VALUES (";
    for (auto value : m_values)
    {
        ss << value.second;
        if (value != m_values.back())
            ss << ", ";
    }
    ss << ")";
    ss << ";";
    return ss.str();

}

/**
 * @brief Add a column to the Creation Query
 * @param columnName Name of the new column
 * @param columnType Column type
 * @param columnContraints Column contrains
 * @return New Query
 */
database::CreateQuery &database::CreateQuery::column(const std::string &columnName, const std::string &columnType, const std::string &columnContraints)
{
    m_valid = true;
    if (!checkColumnName(columnName))
        return *this;
    m_columns.push_back(std::tuple<std::string, std::string, std::string>{columnName, columnType, columnContraints});
    return *this;
}

std::string database::CreateQuery::str() const
{
    if (!isValid())
        return {};
    std::stringstream ss;
    ss << "CREATE TABLE ";
    if (m_ifNotExists)
        ss << "IF NOT EXISTS ";
    ss << m_table;
    ss << " (";
    for (auto column : m_columns)
    {
        ss << std::get<0>(column);
        if (!std::get<1>(column).empty())
            ss << " " << std::get<1>(column);
        if (!std::get<2>(column).empty())
            ss << " " << std::get<2>(column);

        if (column != m_columns.back())
            ss << ", ";
    }
    ss << " ";
    for (auto contraint : m_contraints)
    {
        ss << contraint;
        if (contraint != m_contraints.back())
            ss << ", ";
    }
    ss << ")";
    ss << ";";
    return ss.str();
}

/**
 * @brief Add set couple
 * @param columnName Name of the column to modify
 * @param value New value
 * @return Updated Query
 */
database::UpdateQuery &database::UpdateQuery::set(const std::string &columnName, const std::string &value)
{
    m_valid = true;
    if (!checkColumnName(columnName))
        return *this;
    m_set[columnName] = value;

    return *this;
}

std::string database::UpdateQuery::str() const
{
    if (!isValid())
        return {};
    std::stringstream ss;
    ss << "UPDATE " << m_table << " SET ";

    unsigned int i = 0;
    for (auto value : m_set)
    {
        ss << value.first << " = " << value.second;

        i++;
        if (i < m_set.size())
            ss << ", ";

    }

    if (!m_conditions.empty())
    {
        ss << " WHERE ";
        for (auto condition : m_conditions)
        {
            ss << condition;
            if (condition != m_conditions.back())
                ss << " AND ";
        }
    }

    ss << ";";
    return ss.str();
}
