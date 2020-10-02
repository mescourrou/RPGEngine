#pragma once

// Project
#include "general_config.hpp"
#include <BaseObject.hpp>
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
class DialogueLine : public BaseObject
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
        std::weak_ptr<const DialogueLine> nextLine;
        /// Pointer on the associated action
        std::weak_ptr<DialogueAction> action;
    };

  public:
    /**
     * @brief Default constructor.
     */
    DialogueLine() = default;
    explicit DialogueLine(std::string line);
    DialogueLine(const DialogueLine& copy) = default;
    DialogueLine(DialogueLine&& move) noexcept = default;
    ~DialogueLine() override = default;

    DialogueLine& operator=(const DialogueLine&) = default;
    DialogueLine& operator=(DialogueLine&& move) noexcept = default;

    void loadFromDatabase(unsigned int id,
                          std::shared_ptr<databaseTools::Database> db);

    void addChoice(std::string playerLine,
                   std::weak_ptr<const DialogueLine> nextLine,
                   std::weak_ptr<DialogueAction> action = std::weak_ptr<DialogueAction>());
    void setLine(std::string line);
    const std::string& line() const;
    std::vector<std::string> choices() const;
    std::weak_ptr<const DialogueLine> selectChoice(size_t index) const;

    static bool verifyDatabaseModel(std::shared_ptr<databaseTools::Database> db);
    static bool createDatabaseModel(std::shared_ptr<databaseTools::Database> db);

  private:
    /// NPC line
    std::string m_line;
    /// List of choices for the player
    std::vector<Choice> m_choices;

};

} // namespace quest

