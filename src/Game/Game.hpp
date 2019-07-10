#pragma once

// Std lib
#include <memory>

// Project
#include "general_config.hpp"
#include <BaseObject.hpp>
#include <BaseException.hpp>
#include <Map.hpp>

// External lib
#ifdef RPG_BUILD_TEST
#include <gtest/gtest.h>
#endif

namespace config {
class Context;
}

namespace database {
class Database;
}

namespace character {
class Character;
}

/**
 * @namespace game
 * @brief Group the differents classes to start and manage a game
 */
namespace game
{
#ifdef RPG_BUILD_GUI
namespace GUI {
class GameGUI;
}
#endif

/**
 * @brief Manage the exceptions of Game
 */
CREATE_EXCEPTION_CLASS(Game,
                       ADD_EXCEPTION_CODE(VERSION))

#ifdef RPG_BUILD_TEST
class GameTest;
#endif

/**
 * @brief Manage the game during its execution
 */
class Game : public BaseObject
{
    DECLARE_BASEOBJECT(Game)
#ifdef RPG_BUILD_TEST
    friend class game::GameTest;
#endif
#ifdef RPG_BUILD_GUI
    friend class GUI::GameGUI;
#endif
public:

    Game(std::shared_ptr<config::Context> gameContext);
    /// @brief Destructor
    ~Game() override = default;

    bool initialize(std::shared_ptr<database::Database> db);

    bool run();

    static bool verifyDatabaseModel(std::shared_ptr<database::Database> db);
    static bool createDatabaseModel(std::shared_ptr<database::Database> db);
protected:

    std::shared_ptr<config::Context> m_context;             ///< Context of the Game
    std::shared_ptr<database::Database> m_db;               ///< Database of the Game
    std::shared_ptr<character::Character> m_playerCharacter;  ///< The character played by the player
    std::shared_ptr<map::Map> m_map;                        ///< GUI Map
#ifdef RPG_BUILD_GUI
    std::shared_ptr<game::GUI::GameGUI> m_gui;              ///< GUI pointer
#endif
    bool m_running = true;                                  ///< Main loop condition

};

} // namespace game

