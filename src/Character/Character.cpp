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
    auto result = m_db->query(Query::createQuery<Query::SELECT>(Model::TABLE, m_db)
                              .where(Model::NAME, Query::EQUAL, m_name));
    if (!Database::isQuerySuccessfull(result))
        return false;
    if (result.size() <= 1) // No result
        return false;

    return true;
}

}


