#pragma once

// Project
#include "general_config.hpp"
#include <BaseObject.hpp>
#include <BaseException.hpp>
#include <DialogueLine.hpp>
#include <map>

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
class DialogueTest;
#endif

CREATE_EXCEPTION_CLASS(Dialogue)

/**
 * @brief Manage a complete dialog between the player and an NPC.
 *
 * Expose a static method to load all the Dialogues of one NPC.
 *
 * The Dialogue functionnality is built as follow:
 * - a NPC line is stored by DialogueLine
 * - each DialogueLine can have multiple choices, associated with a player line
 * - to chain multiple NPC lines, you can create empty unique choice
 * - for each link between DialogueLine s, with or without player line, you can associate a DialogueAction.
 *
 * @warning The DialogueAction are not implemented yet.
 *
 * @code
 * std::vector<Dialogue> list = Dialogue::loadFromDatabase("John", database);
 * // Get first line
 * DialogueLine* line = list.at(0).getFirstLine();
 * @endcode
 */
class Dialogue : public BaseObject
{
    DECLARE_BASEOBJECT(Dialogue)
#ifdef RPG_BUILD_TEST
    friend class quest::DialogueTest;
#endif
  public:
    /**
     * @brief Default constructor.
     */
    Dialogue() = default;
    /**
     * @brief Default destructor.
     */
    ~Dialogue() override = default;
    Dialogue(const Dialogue& copy) = default;
    Dialogue(Dialogue&& move) noexcept = default;

    Dialogue& operator=(const Dialogue& copy) = default;
    Dialogue& operator=(Dialogue&& move) noexcept = default;

    Dialogue& loadFromDatabase(unsigned int firstLineID,
                               std::shared_ptr<databaseTools::Database> db);

    /**
     * @brief Get the first DialogueLine of the Dialogue.
     * @return
     */
    std::weak_ptr<const DialogueLine> firstLine() const
    {
        if (m_firstLineId == -1)
            return {};
        return m_dialogueLineStorage.at(m_firstLineId);
    }

    const std::string& characterName() const
    {
        return m_characterName;
    }

    static std::vector<std::shared_ptr<Dialogue>> loadFromDatabase(
                const std::string& NPCName,
                std::shared_ptr<databaseTools::Database> db);

    static bool verifyDatabaseModel(std::shared_ptr<databaseTools::Database> db);
    static bool createDatabaseModel(std::shared_ptr<databaseTools::Database> db);

  private:
    void loadDialogueLineRecursive(unsigned int id,
                                   std::shared_ptr<databaseTools::Database> db);

    /// Pointer on the first dialogue line, for a direct access.
    int m_firstLineId = -1;
    /// Owns all the DialogueLine of the Dialogue.
    std::map<unsigned int, std::shared_ptr<DialogueLine>> m_dialogueLineStorage;
    /// Name of the NPC
    std::string m_characterName;
};

} // namespace quest

