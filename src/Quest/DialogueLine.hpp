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

class DialogueLine : public BaseObject
{
    DECLARE_BASEOBJECT(DialogueLine)
#ifdef RPG_BUILD_TEST
    friend class quest::DialogueLineTest;
#endif
    struct Choice
    {
        std::string playerLine;
        const DialogueLine* m_nextLine = nullptr;
        DialogueAction* m_action = nullptr;
    };

  public:
    DialogueLine() = default;
    DialogueLine(std::string line);
    ~DialogueLine() override;

    void loadFromDatabase(unsigned int id,
                          std::shared_ptr<databaseTools::Database> db);

    void addChoice(std::string playerLine, const DialogueLine* nextLine,
                   DialogueAction* action = nullptr);
    void setLine(std::string line);
    const std::string& getLine() const;
    std::vector<std::string> getChoices() const;
    const DialogueLine* selectChoice(size_t index) const;

    static bool verifyDatabaseModel(std::shared_ptr<databaseTools::Database> db);
    static bool createDatabaseModel(std::shared_ptr<databaseTools::Database> db);

  private:
    std::string m_line;
    std::vector<Choice> m_choices;

};

} // namespace quest

