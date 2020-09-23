#include "GameLoader.hpp"

#include <Game.hpp>
#include <Context.hpp>
#include <Config.hpp>
#include <Database.hpp>
#include <VerbosityLevels.hpp>
#include <ConfigFiles.hpp>
#include <memory>
#include <Character.hpp>
#include <InstrumentationTimer.hpp>

#include <glog/logging.h>

namespace game
{

/**
 * @brief Create a GameLoader
 * @param context Context to use
 */
GameLoader::GameLoader(std::shared_ptr<config::Context> context) :
    m_context(context)
{
    VLOG(verbosityLevel::OBJECT_CREATION) << "Creating " << className() << " => " <<
                                          this;
    m_config.reset(new config::Config(context->gameLocation() + "/" +
                                      config::structure::globalFile::FILE_NAME));
}

/**
 * @brief Load the game : prepare the game initialization
 * @param name Name of the game
 * @return Return false in case of an error
 */
bool GameLoader::load(const std::string& name)
{
    PROFILE_FUNCTION();
    namespace structure = config::structure::globalFile;
    std::string databasePath = m_config->getValue(structure::ressources::SECTION,
                               structure::ressources::DATABASE);
    if (databasePath.empty())
    {
        LOG(ERROR) << "'" << structure::ressources::SECTION << ":" <<
                   structure::ressources::DATABASE << "' field in configuration file not found";
        throw config::ConfigException(std::string() + "'" +
                                      structure::ressources::SECTION + ":" +
                                      structure::ressources::DATABASE + "' field in configuration file not found",
                                      config::ConfigException::LOADING);
    }
    databasePath = m_context->gameLocation() + "/" + databasePath;
    try
    {
        m_db.reset(new databaseTools::Database(databasePath));
    }
    catch (const databaseTools::DatabaseException&)
    {
        throw;
    }

    m_game = std::make_shared<Game>(name, m_context);

    return m_game->initialize(m_db);
}

/**
 * @brief Run the game
 * @return Returns the output of the Game::run
 */
bool GameLoader::run()
{
    PROFILE_FUNCTION();
    return m_game->run();
}

}

