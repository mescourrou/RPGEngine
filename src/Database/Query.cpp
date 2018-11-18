#include "Query.hpp"
#include "Database.hpp"

/**
 * @brief Verify if the column name is valid
 *
 * Current contraints:
 * - No spaces
 *
 * @param name
 * @return
 */
database::DataType database::Query::dataType(const std::string &column)
{
    if (!m_db)
        throw Database::DatabaseException("Query : No database given");
    auto types = m_db->columnsType(m_table);
    if (types.find(column) == types.end())
    {
        LOG(WARNING) << "Requested column (" << column << ") not found in the database";
        return BLOB;
    }

    return m_db->columnsType(m_table).at(column);
}

std::string database::Query::convertToString(database::Query::Operator op)
{
    switch (op) {
    case EQUAL:
        return "=";
    case GT:
        return ">";
    case GE:
        return ">=";
    case LT:
        return "<";
    case LE:
        return "<=";
    case NOT:
        return "NOT";
    }
    return "";
}

bool database::Query::checkColumnName(const std::string &name)
{
    if (name.find(' ') != std::string::npos)
    {
        m_valid = false;
        LOG(ERROR) << "Not valid column name : '" << name << "'";
        return false;
    }
    auto columnList = m_db->columnList(m_table);
    if (std::find(columnList.begin(), columnList.end(), name) == columnList.end())
    {
        m_valid = false;
        LOG(ERROR) << "'" << name << "' column doesn't exists in the table '" << m_table << "'";
        return false;
    }
    return true;
}

void database::Query::doWhere(std::vector<std::string> &conditions, const std::string &column, database::Query::Operator op, std::string value)
{
    if (!checkColumnName(column))
        throw Database::DatabaseException(std::string("Column name not existent in the database : ").append(column));

    auto type = dataType(column);
    if (type == BLOB || type == TEXT)
        value = std::string("'").append(value).append("'");
    conditions.push_back(std::string().append(column).append(" ")
                         .append(convertToString(op)).append(" ").append(value));
}

void database::Query::doColumn(std::vector<std::string> &columns, const std::string &column)
{
    if (!checkColumnName(column))
        throw Database::DatabaseException(std::string("Column name not existent in the database : ").append(column));
    columns.push_back(column);
}

/**
 * @brief Add the couple <column, value> to the query
 * @param column Column of the value
 * @param value Value to insert
 * @return New query
 */
void database::Query::doValue(std::vector<std::pair<std::string, std::string>> &values, const std::string &column, std::string value)
{
    if (!checkColumnName(column))
        throw Database::DatabaseException(std::string("Column name not existent in the database : ").append(column));
    auto type = dataType(column);
    if (type == BLOB || type == TEXT)
        value = std::string("'").append(value).append("'");
    m_valid = true;
    values.push_back(std::pair<std::string, std::string>(column, value));
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
database::CreateQuery &database::CreateQuery::column(const std::string &columnName, DataType columnType, const std::string &columnContraints)
{
    if (!checkColumnName(columnName))
        return *this;
    m_valid = true;
    m_columns.push_back(std::tuple<std::string, DataType, std::string>{columnName, columnType, columnContraints});
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
        ss << " " << Database::convertDataType(std::get<1>(column));
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
    if (!checkColumnName(columnName))
        return *this;
    m_valid = true;
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
