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
 */
class Dialogue : public BaseObject
{
    DECLARE_BASEOBJECT(Dialogue)
#ifdef RPG_BUILD_TEST
    friend class quest::DialogueTest;
#endif
  public:
    Dialogue() = default;
    ~Dialogue() override = default;

    Dialogue& loadFromDatabase(unsigned int firstLineID,
                               std::shared_ptr<databaseTools::Database> db);

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

    DialogueLine* m_firstLine = nullptr;
    std::map<unsigned int, DialogueLine> m_dialogueLineStorage;
};

} // namespace quest

