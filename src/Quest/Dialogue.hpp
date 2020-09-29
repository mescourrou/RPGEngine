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

    Dialogue& loadFromDatabase(unsigned int firstLineID,
                               std::shared_ptr<databaseTools::Database> db);

    /**
     * @brief Get the first DialogueLine of the Dialogue.
     * @return
     */
    DialogueLine* getFirstLine() const
    {
        return m_firstLine;
    }

    static std::vector<Dialogue> loadFromDatabase(std::string NPCName,
            std::shared_ptr<databaseTools::Database> db);

    static bool verifyDatabaseModel(std::shared_ptr<databaseTools::Database> db);
    static bool createDatabaseModel(std::shared_ptr<databaseTools::Database> db);

  private:
    void loadDialogueLineRecursive(unsigned int id,
                                   std::shared_ptr<databaseTools::Database> db);

    /// Pointer on the first dialogue line, for a direct access.
    DialogueLine* m_firstLine = nullptr;
    /// Owns all the DialogueLine of the Dialogue.
    std::map<unsigned int, DialogueLine> m_dialogueLineStorage;
};

} // namespace quest

