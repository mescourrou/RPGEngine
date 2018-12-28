#include "Query.hpp"

// External libs
#include <glog/logging.h>


/**
 * @brief Get the datatype of a column
 * @param [in] column Column to look for
 * @return database::DataType matching
 */
database::DataType database::Query::dataType(const std::string &column)
{
    if (!m_db)
        throw QueryException("No database given", DatabaseException::MISSING_DATABASE);
    auto types = m_db->columnsType(m_table);
    if (types.find(column) == types.end())
    {
        LOG(WARNING) << "Requested column (" << column << ") not found in the database";
        return BLOB;
    }

    return m_db->columnsType(m_table).at(column);
}

/**
 * @brief Convert the operator given in string
 * @param [in] op Operator to convert
 * @return std::string corresponding to the operator
 */
std::string database::Query::operatorAsString(database::Query::Operator op)
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

/**
 * @brief Check if the column name is valid and if the column exists
 * @param [in] name Name of the column to check
 */
void database::Query::checkColumnName(const std::string &name)
{
    if (!checkColumnNameValidity(name))
        throw QueryException(std::string("Column name not valid : ").append(name), QueryException::INVALID_COLUMN_NAME);
    if (!checkColumnExistance(name))
        throw QueryException(std::string("Column name not existent in the database : ").append(name), QueryException::INEXISTANT_COLUMN_NAME);

}

/**
 * @brief Verify if the name is valid
 *
 * Check if the name does not have space.
 *
 * @param [in] name Name to check
 * @return Return true if the name is valid
 */
bool database::Query::checkColumnNameValidity(const std::string &name)
{
    if (name.find(' ') != std::string::npos)
    {
        m_valid = false;
        LOG(ERROR) << "Not valid column name : '" << name << "'";
        return false;
    }
    return true;
}

/**
 * @brief Verify if the column exists
 *
 * If the column doesn't exist, set the validity of the Query to false.
 * @param [in] name Name of the column to check
 * @return Return true if the column exists
 */
bool database::Query::checkColumnExistance(const std::string &name)
{
    auto columnList = m_db->columnList(m_table);
    if (std::find(columnList.begin(), columnList.end(), name) == columnList.end())
    {
        m_valid = false;
        LOG(ERROR) << "'" << name << "' column doesn't exists in the table '" << m_table << "'";
        return false;
    }
    return true;
}

/**
 * @brief Implementation of the adding of conditions to query
 * @param [in,out] conditions Condition vector to modify
 * @param [in] column Column where apply the filter
 * @param [in] op Operator of comparison
 * @param [in] value Value to compare to
 */
void database::Query::doWhere(std::vector<std::string> &conditions, const std::string &column, database::Query::Operator op, std::string value)
{

    auto type = dataType(column);
    if (type == BLOB || type == TEXT)
        value = std::string("'").append(value).append("'");
    conditions.push_back(std::string().append(column).append(" ")
                         .append(operatorAsString(op)).append(" ").append(value));
}

/**
 * @brief Implementation of the add of column in the selection field list
 * @param [in,out] columns Column list to modify
 * @param [in] column Column to add
 */
void database::Query::doColumn(std::vector<std::string> &columns, const std::string &column)
{
    checkColumnName(column);
    columns.push_back(column);
}

/**
 * @brief Add the couple <column, value> to the query
 * @param [in] column Column of the value
 * @param [in] value Value to insert
 * @return New query
 */
void database::Query::doValue(std::vector<std::pair<std::string, std::string>> &values, const std::string &column, std::string value)
{
    checkColumnName(column);
    auto type = dataType(column);
    if (type == BLOB || type == TEXT)
        value = std::string("'").append(value).append("'");
    m_valid = true;
    values.push_back(std::pair<std::string, std::string>(column, value));
}

void database::Query::doSort(std::vector<std::string>& sortColumns, const std::string &column)
{
    checkColumnName(column);
    sortColumns.push_back(column);
}

/**
 * @brief Generate the string corresponding of the Query
 * @return std::string corresponding to the Query
 */
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
    if (m_sortColumns.size() != 0)
    {
        ss << " ORDER BY ";
        for (auto& column : m_sortColumns)
        {
            ss << column;
            if (column != m_sortColumns.back())
                ss << ", ";
        }
        if (m_sortAscending)
            ss << " ASC";
        else
            ss << " DESC";
    }
    ss << ";";
    return ss.str();
}

/**
 * @brief Generate the string corresponding of the Query
 * @return std::string corresponding to the Query
 */
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
 * @param [in] columnName Name of the new column
 * @param [in] columnType Column type
 * @param [in] columnContraints Column contrains
 * @return New Query
 */
database::CreateQuery &database::CreateQuery::column(const std::string &columnName, DataType columnType,
                                                     const std::string& fkTable, const std::string& fkField)
{
    if (!checkColumnNameValidity(columnName))
        return *this;
    if (!fkTable.empty())
    {
        if (fkField.empty())
        {
            m_valid = false;
            return *this;
        }
        if (!m_db->isTable(fkTable))
        {
            m_valid = false;
            return *this;
        }

        auto columnList = m_db->columnList(fkTable);
        if (std::find(columnList.begin(), columnList.end(), fkField) == columnList.end())
        {
            m_valid = false;
            return *this;
        }
    }
    m_valid = true;
    m_columns.push_back(std::tuple<std::string, DataType, std::string, std::string>{
                            columnName, columnType, fkTable, fkField});
    return *this;
}

database::CreateQuery &database::CreateQuery::constraint(const std::string &columnName, database::Query::Constraints constraintType)
{
    if (!checkColumnNameValidity(columnName))
        return *this;

    if (std::find_if(m_columns.begin(), m_columns.end(), [&](std::tuple<std::string, DataType, std::string, std::string> &a) -> bool
                {
                    if (std::get<0>(a) == columnName)
                        return true;
                    return false;
                }
            ) == m_columns.end())
        return *this;

    auto addIfNotFind = [](std::vector<std::string>& list, const std::string& item) {
        if (list.size() == 0 || std::find(list.begin(), list.end(), item) == list.end())
            list.push_back(item);
    };
    switch (constraintType) {
    case Query::Constraints::PRIMARY_KEY:
        addIfNotFind(m_primaryKeyColumns, columnName);
        break;
    case Query::Constraints::UNIQUE:
        addIfNotFind(m_uniqueColumns, columnName);
        break;
    case Query::Constraints::NOT_NULL:
        addIfNotFind(m_notNullColumns, columnName);
        break;
    case Query::Constraints::AUTOINCREMENT:
        addIfNotFind(m_autoincrementColumns, columnName);
        break;
    default:
        LOG(WARNING) << "Constraint type not supported (" << constraintType << ")";
        break;
    }

    return *this;
}

/**
 * @brief Generate the string corresponding of the Query
 * @return std::string corresponding to the Query
 */
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
    for (auto& column : m_columns)
    {
        ss << std::get<0>(column);
        ss << " " << Database::dataTypeAsString(std::get<1>(column));
        if (std::find(m_notNullColumns.begin(), m_notNullColumns.end(), std::get<0>(column)) != m_notNullColumns.end())
            ss << " NOT NULL";
        if (m_primaryKeyColumns.size() == 1)
        {
            if (std::find(m_primaryKeyColumns.begin(), m_primaryKeyColumns.end(), std::get<0>(column)) != m_primaryKeyColumns.end())
                ss << " PRIMARY KEY";
        }
        if (std::find(m_autoincrementColumns.begin(), m_autoincrementColumns.end(), std::get<0>(column)) != m_autoincrementColumns.end())
            ss << " AUTOINCREMENT";
        if (std::find(m_uniqueColumns.begin(), m_uniqueColumns.end(), std::get<0>(column)) != m_uniqueColumns.end())
            ss << " UNIQUE";
        if (!std::get<2>(column).empty())
        {
            ss << " REFERENCES " << std::get<2>(column) << "(`" << std::get<3>(column) << "`)";
        }
        if (column != m_columns.back())
            ss << ", ";
    }
    if (m_primaryKeyColumns.size() > 1)
    {
        ss << ", PRIMARY KEY(";
        for (auto& column : m_primaryKeyColumns)
        {
            ss << "`" << column << "`";
            if (column != m_primaryKeyColumns.back())
                ss << ", ";
        }
        ss << ")";
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
    checkColumnName(columnName);
    m_valid = true;
    m_set[columnName] = value;

    return *this;
}

/**
 * @brief Generate the string corresponding of the Query
 * @return std::string corresponding to the Query
 */
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
