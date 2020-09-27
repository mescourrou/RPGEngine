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

namespace databaseTools
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
    /// Context
    std::shared_ptr<config::Context> m_context;
    /// Game configuration
    std::shared_ptr<config::Config> m_config;
    /// Game database
    std::shared_ptr<databaseTools::Database> m_db;
    /// Game to prepare
    std::shared_ptr<Game> m_game;
};

} // namespace game

