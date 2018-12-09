#pragma once

// Project
#include "general_config.hpp"
#include <BaseObject.hpp>
#include <Database.hpp>
#include <Query.hpp>
#include <Model.hpp>
#include <BaseException.hpp>

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
#endif
public:
    class CharacterException : public BaseException
    {
    public:
        CharacterException(const std::string& w, const Errors& code = BaseException::UNKNOWN):
            BaseException(w, code) {}
    };

    Character(std::string name, std::shared_ptr<database::Database> db);
    virtual ~Character() = default;

    // Getters
    const std::string& name() const noexcept { return m_name; }
    // Setters
    void setName(std::string name) { m_name = std::move(name); }

protected:

    virtual bool loadFromDatabase();

    std::string m_name;
    std::shared_ptr<database::Database> m_db;


};

} // namespace character

