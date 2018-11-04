#pragma once



// Project
#include "general_config.hpp"
#include <BaseObject.hpp>
#include <Query.hpp>

// External libs
#include <glog/logging.h>
#include <sqlite3.h>

#ifdef RPG_BUILD_TEST
#include <gtest/gtest.h>
#endif

/**
 * @namespace database
 * @brief Group all classes usefull for database use
 */
namespace database
{

#ifdef RPG_BUILD_TEST
class DatabaseTest;
#endif

/**
 * @brief Class for Database manipulation
 */
class Database : public BaseObject
{
#ifdef RPG_BUILD_TEST
    friend class database::DatabaseTest;
    FRIEND_TEST(DatabaseTest, QueryText);
#endif
public:
    Database(const std::string& path);
    ~Database() override;

    std::string className() const noexcept override { return "Database"; }

    std::vector<std::map<std::string, std::string>> query(const Query& dbQuery);
    bool query(const std::string& query);

    std::vector<std::string> tableList();
    std::vector<std::string> columnList(const std::string &table);
    std::map<std::string, std::string> columnsType(const std::string &table);
protected:
    int callback(void*, int argc, char** argv, char** colName);

    sqlite3* m_sqlite3Handler = nullptr; ///< Pointer on sqlite handler
    std::shared_ptr<std::vector<std::map<std::string, std::string>>> m_result; ///< Saving results temporary

};


} // namespace config

