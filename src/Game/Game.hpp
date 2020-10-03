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

namespace config
{
class Context;
}

namespace databaseTools
{
class Database;
}

namespace character
{
class Character;
}

/**
 * @namespace game
 * @brief Group the differents classes to start and manage a game
 */
namespace game
{
#ifdef RPG_BUILD_GUI
namespace gui
{
class GameGUI;
}
#endif

/**
 * @brief Manage the exceptions of Game
 */
CREATE_EXCEPTION_CLASS(Game,
                       ADD_EXCEPTION_CODE(VERSION));

#ifdef RPG_BUILD_TEST
class GameTest;
#endif

/**
 * @brief Manage the game during its execution
 */
class Game : public BaseObject
{
    DECLARE_BASEOBJECT(Game);
#ifdef RPG_BUILD_TEST
    friend class game::GameTest;
#endif
#ifdef RPG_BUILD_GUI
    friend class gui::GameGUI;
#endif
  public:

    Game(const std::string& name, std::shared_ptr<config::Context> gameContext);
    /// @brief Destructor
    ~Game() override = default;

    bool initialize(std::shared_ptr<databaseTools::Database> db);

    bool run();

    void loadMapContents(const std::string& mapName);
    void unloadCurrentMap();

    /**
     * @brief Get game name
     */
    const std::string& name() const
    {
        return m_name;
    }

    static bool verifyDatabaseModel(std::shared_ptr<databaseTools::Database> db);
    static bool createDatabaseModel(std::shared_ptr<databaseTools::Database> db);

    std::weak_ptr<game::gui::GameGUI> GUI() const
    {
        return m_gui;
    }

    std::weak_ptr<character::Character> playerCharacter() const
    {
        return m_playerCharacter;
    }

  private:
    /// Context of the Game
    std::shared_ptr<config::Context> m_context;
    /// Database of the Game
    std::shared_ptr<databaseTools::Database> m_db;
    /// The character played by the player
    std::shared_ptr<character::Character> m_playerCharacter;
    /// Pointer on the current map (shortcut to the player character map)
    std::weak_ptr<map::Map> m_currentMap;

    /// Name of the game
    std::string m_name;

    /// List of characters, excepted the character of the player
    std::list<std::shared_ptr<character::Character>> m_characterList;

#ifdef RPG_BUILD_GUI
    /// GUI pointer
    std::shared_ptr<game::gui::GameGUI> m_gui;
#endif
    /// Main loop condition
    bool m_running = true;

};

} // namespace game

