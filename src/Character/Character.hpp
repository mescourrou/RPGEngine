#pragma once

// Project
#include "general_config.hpp"
#include <BaseObject.hpp>
#include <BaseException.hpp>
#include <Position.hpp>
#include <Event.hpp>

#ifdef RPG_BUILD_TEST
#include <gtest/gtest.h>
#endif

#ifdef RPG_BUILD_GUI
#define CHARACTER_GUI_CLASS , public GUI::CharacterGUI
#include <CharacterGUI.hpp>
#else
#define CHARACTER_GUI_CLASS
#endif

namespace database {
class Database;
}

namespace object {
class Inventory;
}

/** @namespace character
 * @brief Group all classes linked to Character business
 */
namespace character
{

/**
 * @brief The CharacterException class group exceptions linked to the Character
 */
CREATE_EXCEPTION_CLASS(Character)

#ifdef RPG_BUILD_TEST
class CharacterTest;
#endif

/**
 * @brief Root character class
 */
class Character : public BaseObject CHARACTER_GUI_CLASS
{
    DECLARE_BASEOBJECT(Character)

#ifdef RPG_BUILD_TEST
    friend class character::CharacterTest;
    FRIEND_TEST(CharacterTest, VerifyDatabaseModel);
    FRIEND_TEST(CharacterTest, LoadingCharacterFromDatabase);
#endif
public:
    Character() = delete;
    Character(std::string name, std::shared_ptr<config::Context> context);
    ~Character() override;

    virtual bool loadFromDatabase(std::shared_ptr<database::Database> db);

    // Getters
    const std::string& name() const noexcept;
    void setPosition(const map::Position& position);
    map::Position position() const;

    // Setters
    /**
     * @brief Set the name of the Character.
     *
     * The name must match the database name during the call of loadFromDatabase
     * @param[in] name New name of the Character
     */
    void setName(std::string name) { m_name = std::move(name); }
    void move(const map::Vector<2> &move);

    static bool verifyDatabaseModel(std::shared_ptr<database::Database> db);
    static bool createDatabaseModel(std::shared_ptr<database::Database> db);

    events::Event<void> signalPositionChanged;
protected:
#ifdef RPG_BUILD_GUI
    void doMove(Direction dir) override;
#endif

    std::shared_ptr<config::Context> m_context;

    std::string m_name;         ///< Name of the Character

    map::Position m_position;   ///< Position of the Character

    std::unique_ptr<object::Inventory> m_inventory; ///< Inventory of the Character

};

} // namespace character

