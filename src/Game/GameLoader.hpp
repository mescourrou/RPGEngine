#pragma once

// Stl
#include <memory>

// Project
#include "general_config.hpp"
#include <BaseObject.hpp>

#ifdef RPG_BUILD_TEST
#include <gtest/gtest.h>
#endif

namespace config
{
class Config;
class Context;
}

namespace database
{
class Database;
}

namespace game
{
class Game;

#ifdef RPG_BUILD_TEST
class GameLoaderTest;
#endif

/**
 * @brief Prepare the start of the Game
 */
class GameLoader : public BaseObject
{
    DECLARE_BASEOBJECT(GameLoader)
#ifdef RPG_BUILD_TEST
    friend class game::GameLoaderTest;
#endif
  public:
    explicit GameLoader(std::shared_ptr<config::Context> context);
    /// @brief Destructor
    ~GameLoader() override = default;

    bool load(const std::string& name);
    bool run();
  private:
    std::shared_ptr<config::Context> m_context; ///< Context
    std::shared_ptr<config::Config> m_config;   ///< Game configuration
    std::shared_ptr<database::Database> m_db;   ///< Game database
    std::shared_ptr<Game> m_game;               ///< Game to prepare
};

} // namespace game

