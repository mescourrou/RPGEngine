#pragma once

// Project
#include "general_config.hpp"
#include <BaseObject.hpp>
#include <BaseException.hpp>

#ifdef RPG_BUILD_TEST
#include <gtest/gtest.h>
#endif

namespace config {
class Config;
class Context;
}

namespace game
{

/**
 * @brief Manage the exceptions for the GameLaucher
 */
class GameLauncherException : public BaseException
{
public:
    static const inline Errors NO_DIRECTORY = Errors(__COUNTER__); ///< The game directory wasn't found
    GameLauncherException(const std::string& w, const Errors& code = BaseException::UNKNOWN):
        BaseException(w, code) {}
};

#ifdef RPG_BUILD_TEST
class GameLauncherTest;
#endif

/**
 * @brief Manage all the games available and launch the choosen one
 */
class GameLauncher : public BaseObject
{
#ifdef RPG_BUILD_TEST
	friend class game::GameLauncherTest;
#endif
public:

    GameLauncher(int argc, char **argv);
    /// @brief Destructor
	~GameLauncher() override = default;

    int start();

	std::string className() const noexcept override { return "GameLauncher"; }

protected:
    bool initialize();

    void startGame(std::string gameName) const;

    std::shared_ptr<config::Context> m_context;     ///< Context

    std::vector<std::string> m_gameList;            ///< List of the founded games

    static constexpr char DIRECTORY_KEY[] = "directory"; ///< Key to find the directory in .ini file
};

} // namespace game

