#pragma once

// Project
#include "general_config.hpp"
#include <BaseObject.hpp>
#include <BaseDatabaseObject.hpp>
#include "DialogueAction.hpp"
#include <BaseException.hpp>

#ifdef RPG_BUILD_TEST
#include <gtest/gtest.h>
#endif

namespace databaseTools
{
class Database;
}

namespace quest
{

#ifdef RPG_BUILD_TEST
class DialogueLineTest;
#endif

CREATE_EXCEPTION_CLASS(DialogueLine)
/**
 * @brief Dialogue NPC line with player choice.
 *
 * About the choices, you can put an empty player line to chain multiple NPC lines.
 */
class DialogueLine : public BaseObject, public BaseDatabaseObject
{
    DECLARE_BASEOBJECT(DialogueLine)
#ifdef RPG_BUILD_TEST
    friend class quest::DialogueLineTest;
#endif
    /**
     * @brief Player choice structure.
     */
    struct Choice
    {
        /// Line of the player
        std::string playerLine;
        /// Pointer on the next NPC line
        const DialogueLine* nextLine = nullptr;
        /// Pointer on the associated action
        DialogueAction* action = nullptr;

        Choice() = default;
        Choice(std::string playerLine_, const DialogueLine* nextLine_,
               DialogueAction* action_) :
            playerLine(std::move(playerLine_)), nextLine(nextLine_), action(action_)
        {}

        Choice(const Choice& copy) = default;

        Choice(Choice&& move) noexcept :
            playerLine(std::move(move.playerLine)), nextLine(move.nextLine),
            action(move.action)
        {
            move.nextLine = nullptr;
            move.action = nullptr;
        }

        Choice& operator=(const Choice&) = default;
        Choice& operator=(Choice&& move) noexcept
        {
            playerLine = std::move(move.playerLine);
            nextLine = move.nextLine;
            action = move.action;
            move.nextLine = nullptr;
            move.action = nullptr;
            return *this;
        }

        ~Choice() = default;
    };

  public:
    /**
     * @brief Default constructor.
     */
    DialogueLine() = default;
    DialogueLine(unsigned int id);
    explicit DialogueLine(std::string line);
    DialogueLine(const DialogueLine& copy) = default;
    DialogueLine(DialogueLine&& move) noexcept;
    ~DialogueLine() override;

    DialogueLine& operator=(const DialogueLine&) = default;
    DialogueLine& operator=(DialogueLine&& move) noexcept;

    bool loadFromDatabase(std::shared_ptr<databaseTools::Database> db) override;

    void addChoice(std::string playerLine, const DialogueLine* nextLine,
                   DialogueAction* action = nullptr);
    void setLine(std::string line);
    const std::string& getLine() const;
    std::vector<std::string> getChoices() const;
    const DialogueLine* selectChoice(size_t index) const;

    void setId(unsigned int id)
    {
        m_id = id;
    }

    unsigned int getId() const
    {
        return m_id;
    }

    bool operator<(const DialogueLine& l2) const
    {
        return getId() < l2.getId();
    }

    static bool verifyDatabaseModel(std::shared_ptr<databaseTools::Database> db);
    static bool createDatabaseModel(std::shared_ptr<databaseTools::Database> db);

  private:
    /// Line id
    unsigned int m_id = 0;
    /// NPC line
    std::string m_line;
    /// List of choices for the player
    std::vector<Choice> m_choices;

};

} // namespace quest

