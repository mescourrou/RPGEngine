#pragma once

// STD lib
#include <mutex>
#include <map>
#include <vector>
#include <memory>


// Project
#include "general_config.hpp"
#include <BaseObject.hpp>
#include <BaseException.hpp>
#include <LuaManager.hpp>

// Libs
#ifdef RPG_BUILD_TEST
#include <gtest/gtest.h>
#endif

class sqlite3;


/**
 * @namespace database
 * @brief Group all classes usefull for database use
 */
namespace database
{

CREATE_EXCEPTION_CLASS(Database,
                       ADD_EXCEPTION_CODE(OPENING) \
                       ADD_EXCEPTION_CODE(MISSING_DATABASE) \
                       ADD_EXCEPTION_CODE(MISSING_TABLE) \
                       ADD_EXCEPTION_CODE(BAD_MODEL))

#ifdef RPG_BUILD_TEST
class DatabaseTest;
#endif
class Query;

/**
 * @brief Possibles types of the data
 */
enum DataType : int
{
    INTEGER,
    TEXT,
    REAL,
    NUMERIC,
    BLOB
};

/**
 * @brief Class for Database manipulation
 */
class Database : public BaseObject
{
    DECLARE_BASEOBJECT(Database)
#ifdef RPG_BUILD_TEST
    friend class database::DatabaseTest;
    FRIEND_TEST(DatabaseTest, QueryText);
#endif
  public:
    explicit Database(const std::string& path);
    ~Database() override;

    std::vector<std::map<std::string, std::string>> query(const Query& dbQuery);
    bool query(const std::string& query);
    static bool isQuerySuccessfull(const
                                   std::vector<std::map<std::string, std::string>>& result);

    /**
     * @brief Return a lockguard of the database mutex
     *
     * It is usefull when you want to use custom string queries
     */
    std::lock_guard<std::mutex> lockGuard()
    {
        return std::lock_guard<std::mutex>(m_queryMutex);
    }

    std::vector<std::string> tableList();
    bool isTable(const std::string& table);
    std::vector<std::string> columnList(const std::string& table);
    std::map<std::string, DataType> columnsType(const std::string& table);

    static std::string dataTypeAsString(const DataType& data);
    static DataType dataTypeFromString(const std::string& data);
  private:
    int callback(int argc, char** argv, char** colName);

    std::mutex m_queryMutex;    ///< Mutex to protect the result access

    sqlite3* m_sqlite3Handler = nullptr; ///< Pointer on sqlite handler
    std::unique_ptr<std::vector<std::map<std::string, std::string>>>
    m_result; ///< Saving results temporary

    const std::vector<std::map<std::string, std::string>>* result() const
    {
        return m_result.get();
    }

    LUA_ADD_BINDING_FOR_OVERLOAD(Database, query, query, bool, const std::string&)
    LUA_ADD_BINDING(Database, result)
};


} // namespace database

