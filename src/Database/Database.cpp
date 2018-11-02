#include "Database.hpp"

database::Database::Database(const std::string &path)
{
    if(sqlite3_open(path.c_str(), &m_sqlite3Handler))
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
    m_result = std::make_shared<std::vector<std::map<std::string, std::string>>>(std::vector<std::map<std::string, std::string>>());
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

std::vector<std::string> database::Database::tableList()
{
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

std::vector<std::string> database::Database::columnList(const std::string& table)
{
    if (query("PRAGMA table_info("+table+");"))
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

std::map<std::string, std::string> database::Database::columnsType(const std::string& table)
{
    if (query("PRAGMA table_info("+table+");"))
    {
        std::map<std::string, std::string> ret;
        for (auto& result : *m_result)
        {
            if (result != m_result->front())
            {
                ret[result.at("name")] = result.at("type");
            }
        }
        return ret;
    }
    else
        return {};
}
