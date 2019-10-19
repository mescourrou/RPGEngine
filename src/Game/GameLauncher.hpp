#pragma once

// Stl
#include <memory>

// Project
#include "general_config.hpp"
#include <BaseObject.hpp>
#include <BaseException.hpp>

#ifdef RPG_BUILD_TEST
#include <gtest/gtest.h>
#endif

namespace config
{
class Config;
class Context;
}

namespace game
{

/**
 * @brief Manage the exceptions for the GameLaucher
 */
CREATE_EXCEPTION_CLASS(GameLauncher,
                       ADD_EXCEPTION_CODE(NO_DIRECTORY))

#ifdef RPG_BUILD_TEST
class GameLauncherTest;
#endif

/**
 * @brief Manage all the games available and launch the choosen one
 */
class GameLauncher : public BaseObject
{
    DECLARE_BASEOBJECT(GameLauncher)
#ifdef RPG_BUILD_TEST
    friend class game::GameLauncherTest;
#endif
  public:

    GameLauncher(int argc, char** argv);
    /// @brief Destructor
    ~GameLauncher() override = default;

    int start();

  protected:
    bool initialize();
    void startGame(std::string gameName) const;
    std::shared_ptr<config::Context> m_context;     ///< Context
    std::vector<std::string> m_gameList;            ///< List of the founded games
};

} // namespace game

