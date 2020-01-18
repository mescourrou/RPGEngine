#include "Character.hpp"

// Project
#include <Database.hpp>
#include <Query.hpp>
#include <Model.hpp>
#include <Inventory.hpp>
#include <VerbosityLevels.hpp>
#include <Map.hpp>
#include <InstrumentationTimer.hpp>

// Extern libs
#include <glog/logging.h>

namespace character
{

/**
 * @brief Construct a character from the database given
 *
 * If a database is given, the loading from the database is performed automatically.
 *
 * @param[in] name Name of the character, must match a name in the database
 * @param[in] context Context to use
 */
Character::Character(const std::string& name,
                     std::shared_ptr<config::Context> context) :
    BaseObject(),
    m_context(context), m_name(name),
    m_inventory(std::make_unique<object::Inventory>())
{
    PROFILE_FUNCTION();
    VLOG(verbosityLevel::OBJECT_CREATION) << "Creating " << className() << " => " <<
                                          this;
    setPosition(m_position);
}

/**
 * @brief Load the character paramaters from the database
 *
 * @param [in] db (Not null) Database to use for loading
 * @return Return true if the loading was successfull
 */
bool Character::loadFromDatabase(std::shared_ptr<database::Database> db)
{
    PROFILE_FUNCTION();
    namespace Model = database::Model::Character;
    using namespace database;
    if (!db)
        throw CharacterException("No database given.",
                                 DatabaseException::MISSING_DATABASE);
    if (!verifyDatabaseModel(db))
        throw CharacterException("The database model is not correct",
                                 DatabaseException::BAD_MODEL);
    auto result = db->query(Query::createQuery<Query::SELECT>(Model::TABLE, db)
                            .where(Model::NAME, Query::EQUAL, m_name));
    if (!Database::isQuerySuccessfull(result))
        return false;
    if (result.size() <= 1) // No result
        return false;

    if (!m_position.loadFromDatabase(db, m_context, m_name))
        return false;

    m_inventory->loadFromDatabase(db, m_name);

    return true;
}

/**
 * @brief Verify if the database has the good character Model
 * @param[in] db Database to modify
 * @return True if the model is correct
 */
bool Character::verifyDatabaseModel(std::shared_ptr<database::Database> db)
{
    PROFILE_FUNCTION();
    namespace Model = database::Model::Character;
    using namespace database;
    if (!db)
        throw CharacterException("No database given.",
                                 DatabaseException::MISSING_DATABASE);
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
 * @brief Add the needed table to the database
 * @param[in] db Database to populate
 * @return Return true if the database was well populated
 */
bool Character::createDatabaseModel(std::shared_ptr<database::Database> db)
{
    PROFILE_FUNCTION();
    namespace Model = database::Model::Character;
    using namespace database;
    if (!db)
        throw CharacterException("No database given.",
                                 DatabaseException::MISSING_DATABASE);

    db->query(Query::createQuery<Query::CREATE>(Model::TABLE, db).ifNotExists()
              .column(Model::NAME).constraint(Model::NAME, Query::PRIMARY_KEY));

    return verifyDatabaseModel(db);
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
 * @brief Set the position of the Character on the map
 * @param position
 */
void Character::setPosition(const map::Position& position)
{
    m_position = position;
}

/**
 * @brief Get the position of the Character
 * @return Position of the Character, constant
 */
map::Position Character::position() const
{
    return m_position;
}

/**
 * @brief Move the Character following the given vector
 * @param move Move to execute
 */
void Character::move(const map::Vector<2>& move)
{
    PROFILE_FUNCTION();
    map::Vector<2> intersection;

    if (m_position.map()->collision({m_position.x(), m_position.y()}, move,
                                    intersection))
    {
        map::Vector<2> newMove{intersection.x() - m_position.x(), intersection.y() - m_position.y()};
        if (newMove.x() >= 1)
            newMove.x() -= 1;
        else if (newMove.x() <= -1)
            newMove.x() += 1;
        else
            newMove.x() = 0;

        if (newMove.y() >= 1)
            newMove.y() -= 1;
        else if (newMove.y() <= -1)
            newMove.y() += 1;
        else
            newMove.y() = 0;

        m_position.x() += newMove.x();
        m_position.y() += newMove.y();
    }
    else
    {
        m_position.x() += move.x();
        m_position.y() += move.y();
    }

    signalPositionChanged.trigger(m_position);
}

}
