#pragma once
#include <memory>

namespace databaseTools
{
class Database;
}

class BaseDatabaseObject
{
  public:
    BaseDatabaseObject() = default;
    virtual ~BaseDatabaseObject() = default;

    virtual
    bool loadFromDatabase(std::shared_ptr<databaseTools::Database> db) = 0;
};

