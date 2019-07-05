#include "Game.hpp"
#include "config.h"

#include <Config.hpp>
#include <Context.hpp>
#include <Database.hpp>
#include <Query.hpp>
#include <Model.hpp>
#include <Character.hpp>
#include <VerbosityLevels.hpp>
#include <MapGUI.hpp>
#include <Money.hpp>

#ifdef RPG_BUILD_GUI
#include <GUI/GameGUI.hpp>
#endif

// External libs
#include <glog/logging.h>
namespace game {

/**
 * @brief Construct a game with a context
 * @param gameContext
 */
Game::Game(std::shared_ptr<config::Context> gameContext) :
    m_context(gameContext), m_gui(std::make_shared<GUI::GameGUI>(m_context))
{
    VLOG(verbosityLevel::OBJECT_CREATION) << "Creating " << className() << " => " << this;
}

/**
 * @brief Initialize the game : load the saved datas, ...
 * @return
 */
bool Game::initialize(std::shared_ptr<database::Database> db)
{
    // Database verification
    LOG(INFO) << "Verify database";
    using namespace database;
    namespace Model = database::Model::Game;
    if (!db)
        throw GameException("No database given.", DatabaseException::MISSING_DATABASE);
    if (!verifyDatabaseModel(db))
        throw GameException("The Game database model is not correct", DatabaseException::BAD_MODEL);
    m_db = db;

    // Get game information
    LOG(INFO) << "Get Game informations";
    auto result = m_db->query(Query::createQuery<Query::SELECT>(Model::TABLE, m_db));
    if (result.size() <= 1)
        throw GameException("There is no Game informations");

    auto gameInfo = result.at(1);

    // Version verification
    if (VERSION < static_cast<unsigned int>(std::atoi(gameInfo.at(Model::ENGINE_VERSION).c_str())))
        throw GameException("Engine version too old for the game", GameException::VERSION);
    LOG(INFO) << "Game version : " << std::atoi(gameInfo.at(Model::ENGINE_VERSION).c_str());

    // Money initialization
    LOG(INFO) << "Initialize Money system";
    object::Money::initializeFromDatabase(db);

    // Create the player character
    LOG(INFO) << "Create the player character";
    m_playerCharacter.reset(new character::Character(gameInfo.at(Model::FK_USER_CHARACTER), m_db));

#ifdef RPG_BUILD_GUI
    LOG(INFO) << "Initialize GUI";
    if (!m_gui->initialize(m_db))
    {
        LOG(ERROR) << "Fail to initialize GUI";
        return false;
    }
    m_gui->setOnClose([this](){ m_running = false; });
#else
    LOG(INFO) << "No GUI initialization because GUI building is not activated";
#endif
    return true;
}

/**
 * @brief Run the game : manage the main loop
 * @return Returns true if all went well
 */
bool Game::run()
{
    LOG(INFO) << "Starting";

    while(m_running)
    {
#ifdef RPG_BUILD_GUI
        // Treat GUI events
        m_gui->eventManager();
#endif

#ifdef RPG_BUILD_GUI
        m_gui->draw();
#endif

    }
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
        throw GameException("No database given.", DatabaseException::MISSING_DATABASE);
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
        else if (column == Model::FIRST_MAP_NAME)
            goodColumns++;
        else
            return false;
    }

    if (goodColumns != 5)
        return false;
    return true;
}

bool Game::createDatabaseModel(std::shared_ptr<database::Database> db)
{
    namespace Model = database::Model::Game;
    using namespace database;
    if (!db)
        throw GameException("No database given.", DatabaseException::MISSING_DATABASE);

    db->query(Query::createQuery<Query::CREATE>(Model::TABLE, db).ifNotExists()
              .column(Model::NAME).constraint(Model::NAME, Query::PRIMARY_KEY)
              .column(Model::VERSION, DataType::INTEGER)
              .column(Model::ENGINE_VERSION, DataType::INTEGER)
              .column(Model::FK_USER_CHARACTER, DataType::BLOB,
                      database::Model::Character::TABLE, database::Model::Character::NAME)
              .column(Model::FIRST_MAP_NAME, DataType::BLOB));

    return verifyDatabaseModel(db);
}

}

