#include "GameLauncher.hpp"
// STL
#include <filesystem>

// Project
#include <general_config.hpp>
#include <GameLoader.hpp>
#include <Context.hpp>
#include <Config.hpp>
#include <Event.hpp>
#include <WorkerThread.hpp>
#include <VerbosityLevels.hpp>
#include <ConfigFiles.hpp>

// External libs
#include <glog/logging.h>

#include <gflags/gflags.h>
DEFINE_int32(verbose, VERBOSE, "Verbosity level");


namespace game {

/**
 * @brief Construct the game launcher
 * @param argc Number of arguments in argv
 * @param argv Program arguments
 */
GameLauncher::GameLauncher(int argc, char **argv)
{
    google::InitGoogleLogging(argv[0]);
    gflags::SetVersionString(std::to_string(VERSION_MAJOR) + "." + std::to_string(VERSION_MINOR) + "."
                             + std::to_string(VERSION_BUILD));
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    if (FLAGS_verbose > 0)
        LOG(INFO) << "Flag verbose : " << FLAGS_verbose;

    google::SetVLOGLevel("*", FLAGS_verbose);
    google::LogToStderr();
    LOG(INFO) << "Starting GameLauncher v" << static_cast<short>(VERSION_MAJOR) << "." <<
                 static_cast<short>(VERSION_MINOR) << "." << static_cast<short>(VERSION_BUILD);
    VLOG(verbosityLevel::OBJECT_CREATION) << "Creating " << className() << " => " << this;
    m_context = std::make_shared<config::Context>(argc, argv);

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
#ifndef GAME_AUTO_CHOOSE
        getline(std::cin, in);
        choice = std::atoi(in.c_str());
#else
        choice = 1;
#endif
        if (choice > 0 && choice - 1 < static_cast<int>(m_gameList.size()))
        {
            startGame(m_gameList.at(static_cast<size_t>(choice-1)));
            choice = 0;
        }
        else if (choice != 0)
            std::cout << "Choose an available option (0 to quit)" << std::endl;
    } while(choice != 0);

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
        std::filesystem::path gameDirectory = m_context->config()->getValue(gameName, config::structure::gameListFile::DIRECTORY_KEY);
        if (gameDirectory.empty())
            throw GameLauncherException("No directory specified", GameLauncherException::NO_DIRECTORY);
        if (!std::filesystem::exists(gameDirectory))
            throw GameLauncherException("Directory non existent : " + gameDirectory.string(), GameLauncherException::NO_DIRECTORY);
        if (!std::filesystem::is_directory(gameDirectory))
            throw GameLauncherException("This is not a directory", GameLauncherException::NO_DIRECTORY);

        m_context->gameLocation() = gameDirectory;

        GameLoader loader(m_context);
        if (!loader.load(gameName))
            return;
        loader.run();
    } catch (const BaseException& e) {
        LOG(ERROR) << "Impossible to start '" << gameName << "' : " << e.what();
    }
    LOG(INFO) << "Exit " << gameName;
}

}

