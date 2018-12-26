#include "Game.hpp"

#include <Config.hpp>
#include <Context.hpp>
#include <Database.hpp>
#include <Query.hpp>
#include <Model.hpp>
#include <Character.hpp>

// External libs
#include <glog/logging.h>
namespace game {

/**
 * @brief Construct a game with a context
 * @param gameContext
 */
Game::Game(std::shared_ptr<config::Context> gameContext) : m_context(gameContext)
{

}

/**
 * @brief Initialize the game : load the saved datas, ...
 * @return
 */
bool Game::initialize(std::shared_ptr<database::Database> db)
{
    using namespace database;
    namespace Model = database::Model::Game;
    if (!db)
        throw GameException("No database given.", Database::DatabaseException::MISSING_DATABASE);
    if (!verifyDatabaseModel(db))
        throw GameException("The database model is not correct", Database::DatabaseException::BAD_MODEL);
    m_db = db;

    auto result = m_db->query(Query::createQuery<Query::SELECT>(Model::TABLE, m_db));
    if (result.size() <= 1)
        return false;

    auto gameInfo = result.at(1);

    if (VERSION < std::atoi(gameInfo.at(Model::ENGINE_VERSION).c_str()))
        throw GameException("Engine version too old for the game", GameException::VERSION);
    m_playerCharacter.reset(new character::Character(gameInfo.at(Model::FK_USER_CHARACTER), m_db));
    return true;
}

/**
 * @brief Run the game : manage the main loop
 * @return Returns true if all went well
 */
bool Game::run()
{
    LOG(INFO) << "Starting";
    return true;
}

/**
 * @brief Verify the database model for initializing the game
 * @param [in] db Database to verify
 * @return Returns true if the database is valid
 */
bool Game::verifyDatabaseModel(std::shared_ptr<database::Database> db)
{
    namespace Model = database::Model::Game;
    using namespace database;
    if (!db)
        throw GameException("No database given.", Database::DatabaseException::MISSING_DATABASE);
    if (!db->isTable(Model::TABLE))
        return false;
    auto columnList = db->columnList(Model::TABLE);

    unsigned short goodColumns = 0;
    for (auto& column : columnList)
    {
        if (column == Model::NAME)
            goodColumns++;
        else if (column == Model::VERSION)
            goodColumns++;
        else if (column == Model::ENGINE_VERSION)
            goodColumns++;
        else if (column == Model::FK_USER_CHARACTER)
            goodColumns++;
        else
            return false;
    }

    if (goodColumns != 4)
        return true;
    return false;
}

}

