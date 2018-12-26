#include "Character.hpp"

// Project
#include <Database.hpp>
#include <Query.hpp>
#include <Model.hpp>
#include <Inventory.hpp>
#include <VerbosityLevels.hpp>

// Extern libs
#include <glog/logging.h>

namespace character {

/**
 * @brief Construct a character from the database given
 *
 * If a database is given, the loading from the database is performed automatically.
 *
 * @param[in] name Name of the character, must match a name in the database
 * @param[in] db [optionnal] Database to use for loading the character.
 */
Character::Character(std::string name, std::shared_ptr<database::Database> db) :
    m_name(std::move(name)), m_inventory(new object::Inventory)
{
    VLOG(verbosityLevel::OBJECT_CREATION) << "Creating " << className() << " => " << this;
    if (db)
        loadFromDatabase(db);
}

Character::~Character()
{
    m_inventory.reset();
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

    m_inventory->loadFromDatabase(db, m_name);


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

/**
 * @brief Get the name of the Character
 * @return Name of the Character
 */
const std::string& Character::name() const noexcept
{
    return m_name;
}

/**
 * @brief Get the position of the Character
 * @return Position of the Character, modifyable
 */
map::Position& Character::position()
{
    return m_position;
}

/**
 * @brief Get the position of the Character
 * @return Position of the Character, constant
 */
map::Position Character::position() const
{
    return m_position;
}

}


