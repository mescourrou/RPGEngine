#include "Game.hpp"
// Stl
#include <thread>
#include <memory>

// Project
#include <general_config.hpp>
#include <Config.hpp>
#include <Context.hpp>
#include <Database.hpp>
#include <Query.hpp>
#include <Model.hpp>
#include <Character.hpp>
#include <VerbosityLevels.hpp>
#include <Money.hpp>
#include <PerformanceTimer.hpp>
#include <InstrumentationTimer.hpp>

#ifdef RPG_BUILD_GUI
#include <GUI/GameGUI.hpp>
#include <GUI/MapGUI.hpp>
#include <GUI/CharacterGUI.hpp>
#endif

// External libs
#include <glog/logging.h>

namespace game
{

/**
 * @brief Construct a game with a context
 * @param gameContext Context
 */
Game::Game(const std::string& name,
           std::shared_ptr<config::Context> gameContext) :
    m_context(gameContext), m_name(name)
#ifdef RPG_BUILD_GUI
    , m_gui(std::make_shared<gui::GameGUI>(m_context, this))
#endif
{
    VLOG(verbosityLevel::OBJECT_CREATION) << "Creating " << className() << " => " <<
                                          this;
}

/**
 * @brief Initialize the game : load the saved datas, ...
 * @param[in] db Database to use for initialization
 * @return Return true if the initialization went well
 */
bool Game::initialize(std::shared_ptr<database::Database> db)
{
    PROFILE_FUNCTION();
    // Database verification
    LOG(INFO) << "Verify database";
    using namespace database;
    namespace Model = database::Model::Game;
    if (!db)
        throw GameException("No database given.", DatabaseException::MISSING_DATABASE);
    if (!verifyDatabaseModel(db))
        throw GameException("The Game database model is not correct",
                            DatabaseException::BAD_MODEL);
    m_db = db;

    // Get game information
    LOG(INFO) << "Get Game informations";
    auto result = m_db->query(Query::createQuery<Query::SELECT>(Model::TABLE,
                              m_db));
    if (result.size() <= 1)
        throw GameException("There is no Game informations");

    auto gameInfo = result.at(1);

    // Version verification
    if (VERSION < static_cast<unsigned int>(std::atoi(gameInfo.at(
            Model::ENGINE_VERSION).c_str())))
        throw GameException("Engine version too old for the game",
                            GameException::VERSION);
    LOG(INFO) << "Game version : " << std::atoi(gameInfo.at(
                  Model::ENGINE_VERSION).c_str());

    // Money initialization
    LOG(INFO) << "Initialize Money system";
    object::Money::initializeFromDatabase(db);

    // Create the player character
    LOG(INFO) << "Create the player character";
    m_playerCharacter = std::make_shared<character::Character>(gameInfo.at(
                            Model::FK_USER_CHARACTER), m_context);
    if (!m_playerCharacter->loadFromDatabase(m_db))
    {
        LOG(ERROR) << "Fail to load the character " << m_playerCharacter->name() <<
                   " from the database";
        throw character::CharacterException(std::string() +
                                            "Fail to load the character " + m_playerCharacter->name() +
                                            " from the database", BaseException::LOADING);
    }
    LOG(INFO) << "Load the map";
    if (!m_playerCharacter->position().map()->load())
    {
        LOG(ERROR) << "Fail to load the map " <<
                   m_playerCharacter->position().map()->name();
        throw character::CharacterException(std::string() + "Fail to load the map " +
                                            m_playerCharacter->position().map()->name(),
                                            BaseException::LOADING);
    }
    m_currentMap = m_playerCharacter->position().map();

#ifdef RPG_BUILD_GUI
    // Initialize the gui
    LOG(INFO) << "Initialize GUI";
    if (!m_gui->initialize(m_db))
    {
        LOG(ERROR) << "Fail to initialize GUI";
        throw game::gui::GameGUIException("Fail to initialize GUI",
                                          BaseException::LOADING);
    }
    m_gui->subscribeOnClose([this]()
    {
        m_running = false;
    });
#else
    LOG(INFO) << "No GUI initialization because GUI building is not activated";
#endif
    loadMapContents(m_currentMap.lock()->name());
    return true;
}

/**
 * @brief Run the game : manage the main loop
 * @return Returns true if all went well
 */
bool Game::run()
{
    PROFILE_FUNCTION();
    LOG(INFO) << "Starting";
    while (m_running)
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
 * @brief Load the elements of the map
 * @param mapName Map to load
 */
void Game::loadMapContents(const std::string& mapName)
{
    PROFILE_FUNCTION();
    using namespace database;

    // Loading the NPCs of the current map
    auto result = m_db->query(Query::createQuery<Query::SELECT>
                              (Model::Position::TABLE, m_db)
                              .where(Model::Position::FK_MAP, Query::EQUAL, mapName)
                              .column(Model::Position::FK_CHARACTER));
    if (!Database::isQuerySuccessfull(result))
    {
        LOG(WARNING) << "Warning : abort loading contents of map " << mapName;
        return;
    }

    for (unsigned int i = 1; i < result.size(); i++)
    {
        auto& characterName = result.at(i).at(Model::Position::FK_CHARACTER);
        if (characterName != m_playerCharacter->name())
        {
            auto& newOne = m_characterList.emplace_back(
                               std::make_shared<character::Character>(characterName, m_context));
            newOne->loadFromDatabase(m_db);
#ifdef RPG_BUILD_GUI
            auto guiChar = m_gui->addGUIObject<character::gui::CharacterGUI>(newOne,
                           m_context);
            guiChar.lock()->load(m_context->kCharacterPath());
            character::gui::CharacterGUI::connectSignals(m_gui.get(), guiChar.lock().get());
            character::gui::CharacterGUI::connectSignals(newOne.get(),
                    guiChar.lock().get());
#endif
        }
    }
}

/**
 * @brief Free all the current map contents
 */
void Game::unloadCurrentMap()
{
    PROFILE_FUNCTION();
    m_characterList.clear();
}

/**
 * @brief Verify the database model for initializing the game
 * @param [in] db Database to verify
 * @return Returns true if the database is valid
 */
bool Game::verifyDatabaseModel(std::shared_ptr<database::Database> db)
{
    PROFILE_FUNCTION();
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

/**
 * @brief Create the table needed for the Game in the database
 * @param[in] db Database to populate
 * @return Return true if the database was well populated
 */
bool Game::createDatabaseModel(std::shared_ptr<database::Database> db)
{
    PROFILE_FUNCTION();
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

