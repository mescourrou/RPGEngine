#pragma once


// Project
#include "general_config.hpp"
#include <BaseObject.hpp>

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

class Query : BaseObject
{
#ifdef RPG_BUILD_TEST
    friend class database::QueryTest;
#endif
public:
    enum QueryTypes{
        SELECT,
        INSERT,
        CREATE,
        UPDATE
    };
protected:
    template<QueryTypes T>
    struct FindQueryType{};
public:
    template<QueryTypes T> static typename FindQueryType<T>::type createQuery(const std::string& table);

    Query(const std::string& table) : m_table(table) {}
    ~Query() override = default;

    virtual std::string str() const = 0;
    virtual bool isValid() const { return m_valid; }

    std::string className() const noexcept override {return "Query";}
protected:

    bool checkColumnName(const std::string& name);

    std::string m_table;
    bool m_valid = false;
};

class SelectQuery : public Query
{
public:
    SelectQuery(const std::string& table) : Query(table) { m_valid = true; }
    ~SelectQuery() override = default;

    SelectQuery& column(const std::string& field) { m_columns.push_back(field); return *this;}
    SelectQuery& where(const std::string& condition) { m_conditions.push_back(condition); return *this;}

    std::string className() const noexcept override {return "SelectQuery";}
    std::string str() const override;

protected:
    std::vector<std::string> m_columns;
    std::vector<std::string> m_conditions;
};

class InsertQuery : public Query
{
public:
    InsertQuery(const std::string& table) : Query(table) {}
    ~InsertQuery() override = default;

    InsertQuery& value(const std::string& column, const std::string& value);

    std::string str() const override;

    std::string className() const noexcept override {return "InsertQuery";}
protected:
    std::vector<std::pair<std::string, std::string>> m_values;
};

class CreateQuery : public Query
{
public:
    CreateQuery(const std::string& table) : Query(table) {}
    ~CreateQuery() override = default;

    CreateQuery& ifNotExists() { m_ifNotExists = true; return *this; }
    CreateQuery& column(const std::string& columnName,
                        const std::string& columnType = "", const std::string& columnContraints = "");
    CreateQuery& contraint(const std::string& contraint) { m_contraints.push_back(contraint); return *this; }

    std::string str() const override;

    std::string className() const noexcept override { return "CreateQuery"; }
protected:
    bool m_ifNotExists = false;

    std::vector<std::tuple<std::string, std::string, std::string>> m_columns;
    std::vector<std::string> m_contraints;

};

class UpdateQuery : public Query
{
public:
    UpdateQuery(const std::string& table) : Query(table) {}
    ~UpdateQuery() override = default;

    UpdateQuery& set(const std::string& columnName, const std::string& value);
    UpdateQuery& where(const std::string& condition) { m_conditions.push_back(condition); return *this; }

    std::string str() const override;

    std::string className() const noexcept override { return "UpdateQuery"; }
protected:
    std::map<std::string, std::string> m_set;
    std::vector<std::string> m_conditions;

};

template<> struct Query::FindQueryType<Query::SELECT> { typedef SelectQuery type;};
template<> struct Query::FindQueryType<Query::INSERT> { typedef InsertQuery type;};
template<> struct Query::FindQueryType<Query::CREATE> { typedef CreateQuery type;};
template<> struct Query::FindQueryType<Query::UPDATE> { typedef UpdateQuery type;};

template<Query::QueryTypes T>
typename Query::FindQueryType<T>::type Query::createQuery(const std::string& table)
{
    return typename Query::FindQueryType<T>::type(table);
}

} // namespace config

