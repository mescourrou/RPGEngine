#include "GameLoader.hpp"

#include <Game.hpp>
#include <Context.hpp>
#include <Config.hpp>
#include <Database.hpp>
#include <VerbosityLevels.hpp>

#include <glog/logging.h>

namespace game {

/**
 * @brief Create a GameLoader
 * @param context Context to use
 */
GameLoader::GameLoader(std::shared_ptr<config::Context> context) :
    m_context(context)
{
    VLOG(verbosityLevel::OBJECT_CREATION) << "Creating " << className() << " => " << this;
    m_config.reset(new config::Config(context->gameLocation() + "/" + configFile::MAIN_CONFIGURATION_FILE));
}

/**
 * @brief Load the game : prepare the game initialization
 * @return Return false in case of an error
 */
bool GameLoader::load()
{
    std::string databasePath = m_config->getValue(configFile::ressources::SECTION,
                                                  configFile::ressources::DATABASE);
    if (databasePath.empty())
    {
        LOG(ERROR) << "'database' field in configuration file not found";
        return false;
    }
    databasePath = m_context->gameLocation() + "/" + databasePath;
    try {
        m_db.reset(new database::Database(databasePath));
    } catch (const database::DatabaseException& e) {
        throw e;
    }

    m_game.reset(new Game(m_context));

    return m_game->initialize(m_db);
}

/**
 * @brief Run the game
 * @return Returns the output of the Game::run
 */
bool GameLoader::run()
{
    return m_game->run();
}

}

