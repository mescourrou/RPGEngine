#include "Database.hpp"
#include <Query.hpp>

/**
 * @brief Create and open the given database file
 * @param path Path to the database
 */
database::Database::Database(const std::string &path)
{
    if(sqlite3_open(path.c_str(), &m_sqlite3Handler))
    {
        LOG(ERROR) << "Can't open database: " << sqlite3_errmsg(m_sqlite3Handler);
        sqlite3_close(m_sqlite3Handler);
        throw DatabaseException("Can't open database", DatabaseException::OPENING);
    }

}

/**
 * @brief Close the database file
 */
database::Database::~Database()
{
    if (m_sqlite3Handler)
    {
        sqlite3_close(m_sqlite3Handler);
    }
}

/**
 * @brief Run a query
 * @param dbQuery Query to execute
 * @return Return the list (std::vector) of row (std::map<column, value>)
 */
std::vector<std::map<std::string, std::string> > database::Database::query(const Query &dbQuery)
{
    std::lock_guard<std::mutex> lock(m_queryMutex);
    query(dbQuery.str());
    return std::move(*m_result);
}

/**
 * @brief Private method which is called at each row match the query
 * @param argc Number of colomns selected
 * @param argv Values
 * @param colName Columns name
 * @return 0
 */
int database::Database::callback(void *, int argc, char **argv, char **colName)
{
    std::map<std::string, std::string> row;
    for(int i = 0; i < argc ; i++)
    {
        row.insert(std::pair<std::string, std::string>(std::string(colName[i]),argv[i] ? argv[i] : "NULL"));
    }
    m_result->emplace_back(std::move(row));
    return 0;
}

/**
 * @brief Run a text query, but do not return the result
 * @param query String query
 * @return True if the query successed and false if it failed
 */
bool database::Database::query(const std::string &query)
{
    if (m_queryMutex.try_lock())
    {
        m_queryMutex.unlock();
        throw DatabaseException("Lock the mutex before the Query");
    }
    m_result = std::make_shared<std::vector<std::map<std::string, std::string>>>(std::vector<std::map<std::string, std::string>>());
    std::map<std::string, std::string> resultRow;
    resultRow["status"] = "fail";
    m_result->emplace_back(std::move(resultRow));
    char *zErrMsg;
    static database::Database* currentDatabase = this; ///< Pointer to the current database to be use in lambdas
    auto cb = [](void *, int argc, char **argv, char **colName) -> int { return currentDatabase->callback(nullptr, argc, argv, colName);};
    DLOG(INFO) << "Execute query : " << query;
    int rc = sqlite3_exec(m_sqlite3Handler, query.c_str(), cb, nullptr, &zErrMsg);
    if( rc!=SQLITE_OK ){
        LOG(ERROR) << "SQL error: " << zErrMsg;
        sqlite3_free(zErrMsg);
        return false;
    }
    m_result->front().at("status") = "success";
    return true;
}

/**
 * @brief List of the database tables
 * @return List of tables
 */
std::vector<std::string> database::Database::tableList()
{
    auto lock = lockGuard();
    if (query("SELECT name FROM sqlite_master WHERE type='table';"))
    {
        std::vector<std::string> tableList;
        for (auto& result : *m_result)
        {
            if (result != m_result->front())
            {
                tableList.push_back(result.at("name"));
            }
        }
        return tableList;
    }
    else
        return {};
}

/**
 * @brief List of the columns of a table
 * @param table Table to search
 * @return List of columns
 */
std::vector<std::string> database::Database::columnList(const std::string& table)
{
    auto lock = lockGuard();
    if (query("PRAGMA table_info('"+table+"');"))
    {
        std::vector<std::string> columnList;
        for (auto& result : *m_result)
        {
            if (result != m_result->front())
            {
                columnList.push_back(result.at("name"));
            }
        }
        return columnList;
    }
    else
        return {};
}

/**
 * @brief Get the type of the columns of a table
 *
 * The types are the SQLite types:
 * - INTERGER
 * - TEXT
 * - NUMBER
 * - REAL
 * - NONE or BLOB
 *
 * @param table Table to search
 * @return Map with first the name of the column and then the name of the type
 */
std::map<std::string, database::DataType> database::Database::columnsType(const std::string& table)
{
    auto lock = lockGuard();
    if (query("PRAGMA table_info('"+table+"');"))
    {
        std::map<std::string, DataType> ret;
        for (auto& result : *m_result)
        {
            if (result != m_result->front())
            {
                ret[result.at("name")] = convertDataType(result.at("type"));
            }
        }
        return ret;
    }
    else
        return {};
}

std::string database::Database::convertDataType(const database::DataType &data)
{
    switch (data) {
    case INTEGER:
        return "INTEGER";
    case TEXT:
        return "TEXT";
    case REAL:
        return "REAL";
    case NUMERIC:
        return "NUMERIC";
    case BLOB:
        return "BLOB";
    default:
        return "BLOB";
    }
}

database::DataType database::Database::convertDataType(const std::string &data)
{
    if (data == "INTEGER")
        return INTEGER;
    if (data == "TEXT")
        return TEXT;
    if (data == "REAL")
        return REAL;
    if (data == "NUMERIC")
        return NUMERIC;
    return BLOB;
}


