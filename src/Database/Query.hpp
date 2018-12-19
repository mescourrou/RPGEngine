#pragma once


// Project
#include "general_config.hpp"
#include <BaseObject.hpp>
#include "Database.hpp"
#include <BaseException.hpp>

// External libs
#include <glog/logging.h>

#ifdef RPG_BUILD_TEST
#include <gtest/gtest.h>
#endif

namespace database
{

#ifdef RPG_BUILD_TEST
class QueryTest;
#endif

class Database;
enum DataType : int;

/**
 * @brief Abstract class for Query generation
 */
class Query : BaseObject
{
#ifdef RPG_BUILD_TEST
    friend class database::QueryTest;
#endif
public:
    class QueryException : public BaseException
    {
    public:
        static const inline Errors INVALID_COLUMN_NAME = Errors(__COUNTER__);
        static const inline Errors INEXISTANT_COLUMN_NAME = Errors(__COUNTER__);
        QueryException(const std::string& w, const Errors& code = BaseException::UNKNOWN) noexcept :
            BaseException(w, code) {}
        ~QueryException() override = default;
    };
    /**
     * @brief Types of Query available
     */
    enum QueryTypes{
        SELECT, ///< Select query : get values
        INSERT, ///< Insert query : add new values
        CREATE, ///< Create table query : create a new table
        UPDATE ///< Update query : edit values
    };

    enum Operator {
        EQUAL,
        GT,
        GE,
        LT,
        LE,
        NOT
    };



protected:
    /**
     * @struct FindQueryType
     * @brief Return the class type associated with the QueryTypes given
     */
    template<QueryTypes T>
    struct FindQueryType{};
public:
    template<QueryTypes T> static typename FindQueryType<T>::type createQuery(const std::string& table, std::shared_ptr<Database> db);
    /// @brief Construct a Query
    Query(const std::string& table, std::shared_ptr<Database> db) : m_table(table), m_db(db) {}
    ~Query() override = default;

    /**
     * @brief Return the query in a string
     *
     * Must be overriden in derived classes
     *
     * @return String
     */
    virtual std::string str() const = 0;
    /**
     * @brief Return if the Query is valid or not. The validity is set in the derived classes
     * @return If the query is not valid, str() will return ""
     */
    virtual bool isValid() const { return m_valid; }

    std::string className() const noexcept override {return "Query";}
protected:
    DataType dataType(const std::string& column);
    std::string operatorAsString(Operator op);
    void checkColumnName(const std::string& name);
    bool checkColumnNameValidity(const std::string& name);
    bool checkColumnExistance(const std::string& name);

    /**
     * @brief Add condition to the condition list
     *
     * Use with caution, there is no verification
     * @param [in,out] conditions Condition list to modify
     * @param [in] condition Condition to add
     */
    virtual void doWhere(std::vector<std::string>& conditions, const std::string& condition) final { conditions.push_back(condition);}
    virtual void doWhere(std::vector<std::string>& conditions, const std::string& column, Operator op, std::string value) final;
    virtual void doColumn(std::vector<std::string>& columns, const std::string& column) final;
    virtual void doValue(std::vector<std::pair<std::string, std::string>> &values, const std::string &column, std::string value) final;

    std::string m_table; ///< Name of the table targeted by the Query
    std::shared_ptr<Database> m_db; ///< Database where the Query will apply (used for verifications)
    bool m_valid = false; ///< Validity of the Query

};

/**
 * @brief Create a SELECT Query
 */
class SelectQuery : public Query
{
public:
    /// @brief Construct a SELECT Query
    SelectQuery(const std::string& table, std::shared_ptr<Database> db) : Query(table, db) { m_valid = true; }
    ~SelectQuery() override = default;

    /// @brief Add a selected column
    SelectQuery& column(const std::string& field) { doColumn(m_columns, field); return *this;}
    /// @brief Add a filter condition
    SelectQuery& where(const std::string& condition) { doWhere(m_conditions, condition); return *this;}
    /// @brief Add a filter condition
    SelectQuery& where(const std::string& column, Operator op, const std::string& value) { doWhere(m_conditions, column, op, value); return *this;}


    std::string className() const noexcept override {return "SelectQuery";}
    std::string str() const override;

protected:
    std::vector<std::string> m_columns; ///< Columns selected
    std::vector<std::string> m_conditions; ///< Selection conditions
};

/**
 * @brief Create an INSERT Query
 */
class InsertQuery : public Query
{
public:
    /// @brief Construct an INSERT Query
    InsertQuery(const std::string& table, std::shared_ptr<Database> db) : Query(table, db) {}
    ~InsertQuery() override = default;

    /// @brief Add a value to the adding list
    InsertQuery& value(const std::string& column, const std::string& value) { doValue(m_values, column, value); return *this; }

    std::string str() const override;

    std::string className() const noexcept override {return "InsertQuery";}
protected:
    std::vector<std::pair<std::string, std::string>> m_values; ///< Values to insert
};

/**
 * @brief Create an CREATE TABLE Query
 */
class CreateQuery : public Query
{
public:
    /// @brief Construct a CREATE TABLE Query
    CreateQuery(const std::string& table, std::shared_ptr<Database> db) : Query(table, db) {}
    ~CreateQuery() override = default;

    /// @brief Add "IF NOT EXISTS" statement to the create table query
    CreateQuery& ifNotExists() { m_ifNotExists = true; return *this; }
    CreateQuery& column(const std::string& columnName,
                        DataType columnType = DataType::BLOB, const std::string& columnContraints = "");
    /// @brief Add the table contraint
    CreateQuery& contraint(const std::string& contraint) { m_contraints.push_back(contraint); return *this; }

    std::string str() const override;

    std::string className() const noexcept override { return "CreateQuery"; }
protected:
    bool m_ifNotExists = false; ///< "IF NOT EXISTS" statement add

    /**
     * @brief Columns to create
     *
     * The tuple contains :
     * - the column name
     * - the type of the column
     * - column contraints, like primary, unique, ...
     */
    std::vector<std::tuple<std::string, DataType, std::string>> m_columns;
    /**
     * @brief Contrains of the table, like primary, unique, ...
     */
    std::vector<std::string> m_contraints;

};

/**
 * @brief Create an UPDATE query
 */
class UpdateQuery : public Query
{
public:
    /// @brief Construct a UPDATE Query
    UpdateQuery(const std::string& table, std::shared_ptr<Database> db) : Query(table, db) {}
    ~UpdateQuery() override = default;

    UpdateQuery& set(const std::string& columnName, const std::string& value);
    /// @brief Add filter condition
    UpdateQuery& where(const std::string& condition) { doWhere(m_conditions, condition); return *this; }

    std::string str() const override;

    std::string className() const noexcept override { return "UpdateQuery"; }
protected:
    std::map<std::string, std::string> m_set; ///< Couples column name / new value
    std::vector<std::string> m_conditions; ///< Filter for update

};

/// QueryTypes::SELECT into SelectQuery
template<> struct Query::FindQueryType<Query::SELECT> { typedef SelectQuery type; /**< Type matching QueryTypes::SELECT */ };
/// QueryTypes::INSERT into InsertQuery
template<> struct Query::FindQueryType<Query::INSERT> { typedef InsertQuery type; /**< Type matching QueryTypes::INSERT */ };
/// QueryTypes::CREATE into CreateQuery
template<> struct Query::FindQueryType<Query::CREATE> { typedef CreateQuery type; /**< Type matching QueryTypes::CREATE */ };
/// QueryTypes::UPDATE into UpdateQuery
template<> struct Query::FindQueryType<Query::UPDATE> { typedef UpdateQuery type; /**< Type matching QueryTypes::UPDATE */ };

/**
 * @fn Query::FindQueryType<T>::type Query::createQuery(const std::string& table)
 * @brief Create the QUery which match the QueryType given
 * @param table Name of the table on which the query will be applied
 */
template<Query::QueryTypes T>
typename Query::FindQueryType<T>::type Query::createQuery(const std::string& table, std::shared_ptr<Database> db)
{
    return typename Query::FindQueryType<T>::type(table, db);
}

} // namespace config

