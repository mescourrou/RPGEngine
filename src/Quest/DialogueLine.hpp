#pragma once

// Project
#include "general_config.hpp"
#include <BaseObject.hpp>
#include "DialogueAction.hpp"

#ifdef RPG_BUILD_TEST
#include <gtest/gtest.h>
#endif

namespace quest
{

#ifdef RPG_BUILD_TEST
class DialogueLineTest;
#endif

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

    void addChoice(std::string playerLine, const DialogueLine* nextLine,
                   DialogueAction* action = nullptr);
    void setLine(std::string line);
    const std::string& getLine() const;
    std::vector<std::string> getChoices() const;
    const DialogueLine* selectChoice(size_t index) const;


  private:
    std::string m_line;
    std::vector<Choice> m_choices;

};

} // namespace quest

