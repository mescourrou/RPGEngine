#pragma once

// Project
#include "general_config.hpp"
#include <BaseObject.hpp>
#include <BaseException.hpp>
#include <Position.hpp>

#ifdef RPG_BUILD_TEST
#include <gtest/gtest.h>
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
class CharacterException : public BaseException
{
public:
    CharacterException(const std::string& w, const Errors& code = BaseException::UNKNOWN):
        BaseException(w, code) {}
};

#ifdef RPG_BUILD_TEST
class CharacterTest;
#endif

/**
 * @brief Root character class
 */
class Character : public BaseObject
{
#ifdef RPG_BUILD_TEST
    friend class character::CharacterTest;
    FRIEND_TEST(CharacterTest, VerifyDatabaseModel);
    FRIEND_TEST(CharacterTest, LoadingCharacterFromDatabase);
#endif
public:
    Character() = delete;
    Character(std::string name, std::shared_ptr<database::Database> db = nullptr);
    virtual ~Character();

    virtual bool loadFromDatabase(std::shared_ptr<database::Database> db);

    // Getters
    const std::string& name() const noexcept;
    map::Position& position();
    map::Position position() const;

    // Setters
    /**
     * @brief Set the name of the Character.
     *
     * The name must match the database name during the call of loadFromDatabase
     * @param[in] name New name of the Character
     */
    void setName(std::string name) { m_name = std::move(name); }


    std::string className() const noexcept override { return "Character"; }

    static bool verifyDatabaseModel(std::shared_ptr<database::Database> db);
    static bool createDatabaseModel(std::shared_ptr<database::Database> db);
protected:

    std::string m_name;         ///< Name of the Character

    map::Position m_position;   ///< Position of the Character

    std::unique_ptr<object::Inventory> m_inventory; ///< Inventory of the Character

};

} // namespace character

