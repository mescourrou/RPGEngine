#pragma once

// Project
#include "general_config.hpp"
#include <BaseObject.hpp>

#ifdef RPG_BUILD_TEST
#include <gtest/gtest.h>
#endif

namespace config {
class Config;
class Context;
}

namespace database {
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
    GameLoader(std::shared_ptr<config::Context> context);
    /// @brief Destructor
	~GameLoader() override = default;

    bool load();
    bool run();
protected:
    std::shared_ptr<config::Context> m_context; ///< Context
    std::shared_ptr<config::Config> m_config;   ///< Game configuration
    std::shared_ptr<database::Database> m_db;   ///< Game database
    std::shared_ptr<Game> m_game;               ///< Game to prepare

    /**
     * @brief Contains the structure of the .ini file
     */
    struct configFile {
    static constexpr char MAIN_CONFIGURATION_FILE[] = "main.ini";
    struct ressources {
    static constexpr char SECTION[] = "Ressources";
    static constexpr char DATABASE[] = "database";
    };
    };
};

} // namespace game

