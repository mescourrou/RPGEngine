#include "Maker.hpp"

#include <Database.hpp>
#include <Model.hpp>

// Character
#include <Character.hpp>

// Object
#include <Object.hpp>
#include <Inventory.hpp>
#include <Money.hpp>
#include <Context.hpp>
#include <Game.hpp>

namespace maker {

Maker::Maker()
{

}

void Maker::loadDatabase(const std::string &filename)
{
    try {
        m_db.reset(new database::Database(filename));
    } catch (const database::DatabaseException& e) {
        if (e.code() == database::DatabaseException::OPENING)
        {
            m_signalDatabaseLoaded.trigger(DatabaseStatus::NOT_LOADED);
            return;
        }
        throw e;
    }

    if (m_db->tableList().size() == 0)
    {
        m_signalDatabaseLoaded.trigger(DatabaseStatus::EMPTY);
        return;
    }
    if (verifyDatabaseModel(m_db))
        m_signalDatabaseLoaded.trigger(DatabaseStatus::VALID);
    else
        m_signalDatabaseLoaded.trigger(DatabaseStatus::NOT_VALID);
}

bool Maker::verifyDatabaseModel(std::shared_ptr<database::Database> db)
{
    return
            character::Character::verifyDatabaseModel(db) &&
            object::Object::verifyDatabaseModel(db) &&
            object::Money::verifyDatabaseModel(db) &&
            object::Inventory::verifyDatabaseModel(db) &&
            map::Position::verifyDatabaseModel(db) &&
            game::Game::verifyDatabaseModel(db);
}

bool Maker::createDatabaseModel()
{
    if (!m_db)
        throw MakerException("No database loaded.", database::DatabaseException::MISSING_DATABASE);
    return
            character::Character::createDatabaseModel(m_db) &&
            map::Position::createDatabaseModel(m_db) &&
            object::Object::createDatabaseModel(m_db) &&
            object::Money::createDatabaseModel(m_db) &&
            object::Inventory::createDatabaseModel(m_db) &&
            game::Game::createDatabaseModel(m_db);
}

}
