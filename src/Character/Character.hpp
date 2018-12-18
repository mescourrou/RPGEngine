#pragma once

// Project
#include "general_config.hpp"
#include <BaseObject.hpp>
#include <Database.hpp>
#include <Query.hpp>
#include <Model.hpp>
#include <BaseException.hpp>
#include <Position.hpp>

// Extern libs
#include <glog/logging.h>

#ifdef RPG_BUILD_TEST
#include <gtest/gtest.h>
#endif

/** @namespace character
 * @brief Group all classes linked to Character business
 */
namespace character
{

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
    /**
     * @brief The CharacterException class group exceptions linked to the Character
     */
    class CharacterException : public BaseException
    {
    public:
        CharacterException(const std::string& w, const Errors& code = BaseException::UNKNOWN):
            BaseException(w, code) {}
    };

    Character(std::string name, std::shared_ptr<database::Database> db = {});
    virtual ~Character() = default;

    virtual bool loadFromDatabase(std::shared_ptr<database::Database> db);

    // Getters
    /**
     * @brief Get the name of the Character
     * @return Name of the Character
     */
    const std::string& name() const noexcept { return m_name; }

    /**
     * @brief Get the position of the Character
     * @return Position of the Character, modifyable
     */
    map::Position& position() { return m_position; }

    /**
     * @brief Get the position of the Character
     * @return Position of the Character, constant
     */
    map::Position position() const { return m_position; }


    // Setters
    /**
     * @brief Set the name of the Character.
     *
     * The name must match the database name during the call of loadFromDatabase
     * @param[in] name New name of the Character
     */
    void setName(std::string name) { m_name = std::move(name); }


    std::string className() const noexcept override { return "Character"; }

protected:
    static bool verifyDatabaseModel(std::shared_ptr<database::Database> db);

    std::string m_name;         ///< Name of the Character

    map::Position m_position;   ///< Position of the Character


};

} // namespace character

