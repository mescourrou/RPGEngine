#include "Character.hpp"

// Project
#include <Database.hpp>
#include <Query.hpp>
#include <Model.hpp>
#include <Inventory.hpp>
#include <VerbosityLevels.hpp>
#include <Map.hpp>

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
Character::Character(std::string name, std::shared_ptr<config::Context> context) :
    m_name(std::move(name)), m_context(context), m_inventory(std::make_unique<object::Inventory>())
{
    VLOG(verbosityLevel::OBJECT_CREATION) << "Creating " << className() << " => " << this;
    setPosition(m_position);
}

/**
 * @brief Destructor
 */
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
        throw CharacterException("No database given.", DatabaseException::MISSING_DATABASE);
    if (!verifyDatabaseModel(db))
        throw CharacterException("The database model is not correct", DatabaseException::BAD_MODEL);
    auto result = db->query(Query::createQuery<Query::SELECT>(Model::TABLE, db)
                              .where(Model::NAME, Query::EQUAL, m_name));
    if (!Database::isQuerySuccessfull(result))
        return false;
    if (result.size() <= 1) // No result
        return false;

    if (!m_position.loadFromDatabase(db, m_context, m_name))
        return false;

    m_inventory->loadFromDatabase(db, m_name);

#ifdef RPG_BUILD_GUI
    if (!GUI::CharacterGUI::load(m_name, m_context->kCharacterPath()))
    {
        LOG(ERROR) << "Error during loading the GUI elements of " << m_name;
        return false;
    }
#endif

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
        throw CharacterException("No database given.", DatabaseException::MISSING_DATABASE);
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
    namespace Model = database::Model::Character;
    using namespace database;
    if (!db)
        throw CharacterException("No database given.", DatabaseException::MISSING_DATABASE);

    db->query(Query::createQuery<Query::CREATE>(Model::TABLE, db).ifNotExists()
              .column(Model::NAME).constraint(Model::NAME, Query::PRIMARY_KEY));

    return verifyDatabaseModel(db);
}

#ifdef RPG_BUILD_GUI
void Character::doMove(GUI::CharacterGUI::Direction dir)
{
    switch (dir) {
    case Up:
        move({0, -5});
        break;
    case Down:
        move({0, 5});
        break;
    case Left:
        move({-5,0});
        break;
    case Right:
        move({5,0});
        break;
    }
}
#endif

/**
 * @brief Get the name of the Character
 * @return Name of the Character
 */
const std::string& Character::name() const noexcept
{
    return m_name;
}

void Character::setPosition(const map::Position &position)
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

void Character::move(const map::Vector<2>& move)
{
    map::Vector<2> intersection;

    if (m_position.map()->collision({m_position.x(), m_position.y()}, move, intersection))
    {
        if (intersection != map::Vector<2>{-1, -1})
        {
            if (move.x() > 0)
                intersection.x() -= 1;
            else if (move.x() < 0)
                intersection.x() += 1;
            if (move.y() > 0)
                intersection.y() -= 1;
            else if (move.y() < 0)
                intersection.y() += 1;
            m_position.x() = intersection.x();
            m_position.y() = intersection.y();
        }
    }
    else
    {
        m_position.x() += move.x();
        m_position.y() += move.y();
    }

    signalPositionChanged.trigger();
}

}
