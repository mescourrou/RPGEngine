#include "Character.hpp"

namespace character {

Character::Character(std::string name, std::shared_ptr<database::Database> db) : m_name(std::move(name)), m_db(db)
{
    if (!m_db)
        throw CharacterException("No database given.", database::Database::DatabaseException::MISSING_DATABASE);
}

bool Character::loadFromDatabase()
{
    if (!m_db)
        throw CharacterException("Unexpected : No database.", BaseException::UNKNOWN);



    return true;
}

}


