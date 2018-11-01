#include "Database.hpp"

database::Database::Database(const std::string &path)
{
    int rc = sqlite3_open(path.c_str(), &m_sqlite3Handler);
    if(rc)
    {
        LOG(ERROR) << "Can't open database: " << sqlite3_errmsg(m_sqlite3Handler);
        sqlite3_close(m_sqlite3Handler);
        throw std::string("Can't open database");
    }

}

database::Database::~Database()
{
    if (m_sqlite3Handler)
    {
        sqlite3_close(m_sqlite3Handler);
    }
}

std::vector<std::map<std::string, std::string> > database::Database::query(const Query &dbQuery)
{
    query(dbQuery.str());
    return std::move(*m_result);
}


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

static database::Database* currentDatabase = nullptr;

bool database::Database::query(const std::string &query)
{
    m_result = std::shared_ptr<std::vector<std::map<std::string, std::string>>>(new std::vector<std::map<std::string, std::string>>);
    std::map<std::string, std::string> resultRow;
    resultRow["status"] = "fail";
    m_result->emplace_back(std::move(resultRow));
    char *zErrMsg;
    currentDatabase = this;
    auto cb = [](void *, int argc, char **argv, char **colName) -> int { return currentDatabase->callback(nullptr, argc, argv, colName);};
    int rc = sqlite3_exec(m_sqlite3Handler, query.c_str(), cb, nullptr, &zErrMsg);
    if( rc!=SQLITE_OK ){
        LOG(ERROR) << "SQL error: " << zErrMsg;
        sqlite3_free(zErrMsg);
        return false;
    }
    m_result->front().at("status") = "success";
    return true;
}
