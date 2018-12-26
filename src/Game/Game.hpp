#pragma once

// Std lib
#include <memory>
#include <filesystem>

// Project
#include "general_config.hpp"
#include <BaseObject.hpp>
#include <BaseException.hpp>

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

#ifdef RPG_BUILD_TEST
class GameTest;
#endif

/**
 * @brief Manage the game during its execution
 */
class Game : public BaseObject
{
#ifdef RPG_BUILD_TEST
	friend class game::GameTest;
#endif
public:
    /**
     * @brief Manage the exceptions of Game
     */
    class GameException : public BaseException
    {
    public:
        static const inline Errors VERSION = Errors(__COUNTER__);
        GameException(const std::string& w, const Errors& code = BaseException::UNKNOWN):
            BaseException(w, code) {}
    };
    Game(std::shared_ptr<config::Context> gameContext);
    /// @brief Destructor
	~Game() override = default;

    bool initialize(std::shared_ptr<database::Database> db);

    bool run();
	
	std::string className() const noexcept override { return "Game"; }

private:
    static bool verifyDatabaseModel(std::shared_ptr<database::Database> db);

    std::shared_ptr<config::Context> m_context;             ///< Context of the Game
    std::shared_ptr<database::Database> m_db;               ///< Database of the Game
    std::shared_ptr<character::Character> m_playerCharacter;  ///< The character played by the player

};

} // namespace game
