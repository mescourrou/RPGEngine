#include "Character.hpp"

namespace character {

Character::Character(std::string name, std::shared_ptr<database::Database> db) : m_name(std::move(name)), m_db(db)
{
    if (!m_db)
        throw CharacterException("No database given.", database::Database::DatabaseException::MISSING_DATABASE);
    if (!m_db->isTable(database::Model::Character::TABLE))
        throw CharacterException("Missing Character table", database::Database::DatabaseException::MISSING_TABLE);

}

bool Character::loadFromDatabase()
{
    namespace Model = database::Model::Character;
    using namespace database;
    if (!m_db)
        throw CharacterException("Unexpected : No database.", BaseException::UNKNOWN);
    if (!verifyDatabaseModel(m_db))
        throw CharacterException("The database model is not correct", Database::DatabaseException::BAD_MODEL);
    auto result = m_db->query(Query::createQuery<Query::SELECT>(Model::TABLE, m_db)
                              .where(Model::NAME, Query::EQUAL, m_name));
    if (!Database::isQuerySuccessfull(result))
        return false;
    if (result.size() <= 1) // No result
        return false;

    if (!m_position.loadFromDatabase(m_db, m_name))
        return false;

    return true;
}

bool Character::verifyDatabaseModel(std::shared_ptr<database::Database> db)
{
    namespace Model = database::Model::Character;
    using namespace database;
    if (!db->isTable(Model::TABLE))
        return false;
    auto columnList = db->columnList(Model::TABLE);

    unsigned short goodColumns = 0;
    for (auto& column : columnList)
    {
        if (column == Model::NAME)
            goodColumns++;
        else
            return false;
    }

    if (goodColumns == 1)
        return true;
    return false;
}

}


