#include "GameLauncher.hpp"
// STL
#include <filesystem>

#include <GameLoader.hpp>
#include <Context.hpp>
#include <Config.hpp>
#include <Event.hpp>
#include <WorkerThread.hpp>
#include <VerbosityLevels.hpp>

// External libs
#include <glog/logging.h>

namespace game {

/**
 * @brief Construct the game launcher
 * @param argc Number of arguments in argv
 * @param argv Program arguments
 */
GameLauncher::GameLauncher(int argc, char **argv)
{
    google::InitGoogleLogging(argv[0]);
    google::SetVLOGLevel("*", VERBOSE);
    google::LogToStderr();
    VLOG(verbosityLevel::OBJECT_CREATION) << "Creating " << className() << " => " << this;
    m_context.reset(new config::Context(argc, argv));

}

/**
 * @brief Start the launcher
 * @return Returns 0 if the user quit properly
 */
int GameLauncher::start()
{
    if (!initialize())
        return -2;
    int choice = -1;
    events::Event<std::string> eventStartingGame;
    eventStartingGame.subscribeSync(std::bind(&GameLauncher::startGame, this, std::placeholders::_1));
    do
    {
        std::cout << "Available games : " << std::endl;
        for (unsigned int i = 0; i < m_gameList.size(); i++)
        {
            std::cout <<  std::setfill('0') << std::setw(2) << i+1;
            std::cout << " -> " << m_gameList.at(i) << std::endl;
        }
        std::cout << "Which game do you choose ? : ";
        std::string in;
        //getline(std::cin, in);
        //choice = std::atoi(in.c_str());
        choice = 1;
        if (choice > 0 && choice - 1 < m_gameList.size())
        {
            eventStartingGame.trigger(m_gameList.at(choice-1));
        }
        else if (choice != 0)
            std::cout << "Choose an available option (0 to quit)" << std::endl;
        choice = 0;
    }while(choice != 0);
    return 0;
}

/**
 * @brief Initialize the game launcher
 * @return Return true if all went well
 */
bool GameLauncher::initialize()
{
    m_gameList = m_context->config()->getAllSections();

    return true;
}

/**
 * @brief Start the game given
 * @param [in] gameName NameVERBOSE of the game to start
 */
void GameLauncher::startGame(std::string gameName) const
{
    LOG(INFO) << "Starting " << gameName;
    try {
        std::filesystem::path gameDirectory = m_context->config()->getValue(gameName, DIRECTORY_KEY);
        if (gameDirectory.empty())
            throw GameLauncherException("No directory specified", GameLauncherException::NO_DIRECTORY);
        if (!std::filesystem::exists(gameDirectory))
            throw GameLauncherException("Directory non existent", GameLauncherException::NO_DIRECTORY);
        if (!std::filesystem::is_directory(gameDirectory))
            throw GameLauncherException("This is not a directory", GameLauncherException::NO_DIRECTORY);

        m_context->gameLocation() = gameDirectory;

        GameLoader loader(m_context);
        if (!loader.load())
            return;
        loader.run();
    } catch (const BaseException& e) {
        LOG(ERROR) << "Impossible to start '" << gameName << "' : " << e.what();
    }
    LOG(INFO) << "Exit " << gameName;
}

}

