#include "Character.hpp"

namespace character {

/**
 * @brief Construct a character from the database given
 *
 * If a database is given, the loading from the database is performed automatically.
 *
 * @param[in] name Name of the character, must match a name in the database
 * @param[in] db [optionnal] Database to use for loading the character.
 */
Character::Character(std::string name, std::shared_ptr<database::Database> db) : m_name(std::move(name))
{
    if (db)
        loadFromDatabase(db);
}

/**
 * @brief Load the character paramaters from the database
 *
 * @param [in] db (Not null) Database to use for loading
 * @return Return true if the loading was successfull
 */
bool Character::loadFromDatabase(std::shared_ptr<database::Database> db)
{
    namespace Model = database::Model::Character;
    using namespace database;
    if (!db)
        throw CharacterException("No database given.", database::Database::DatabaseException::MISSING_DATABASE);
    if (!verifyDatabaseModel(db))
        throw CharacterException("The database model is not correct", Database::DatabaseException::BAD_MODEL);
    auto result = db->query(Query::createQuery<Query::SELECT>(Model::TABLE, db)
                              .where(Model::NAME, Query::EQUAL, m_name));
    if (!Database::isQuerySuccessfull(result))
        return false;
    if (result.size() <= 1) // No result
        return false;

    if (!m_position.loadFromDatabase(db, m_name))
        return false;

    return true;
}

/**
 * @brief Verify if the database has the good character Model
 * @param[in] db Database to modify
 * @return
 */
bool Character::verifyDatabaseModel(std::shared_ptr<database::Database> db)
{
    namespace Model = database::Model::Character;
    using namespace database;
    if (!db)
        throw CharacterException("No database given.", database::Database::DatabaseException::MISSING_DATABASE);
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


