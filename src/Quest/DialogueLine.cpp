#include "DialogueLine.hpp"
namespace quest
{

DialogueLine::DialogueLine(std::string line) : m_line(std::move(line))
{

}

DialogueLine::~DialogueLine()
{
    for (auto& choice : m_choices)
    {
        if (choice.m_action)
            delete choice.m_action;
    }
}

void DialogueLine::addChoice(std::string playerLine,
                             const DialogueLine* nextLine, DialogueAction* action)
{
    m_choices.push_back(Choice{std::move(playerLine), nextLine, action});
}

void DialogueLine::setLine(std::string line)
{
    m_line = std::move(line);
}

const std::string& DialogueLine::getLine() const
{
    return m_line;
}

std::vector<std::string> DialogueLine::getChoices() const
{
    std::vector<std::string> choices;
    for (auto& choice : m_choices)
    {
        choices.push_back(choice.playerLine);
    }
    return choices;
}

const DialogueLine* DialogueLine::selectChoice(size_t index) const
{
    const Choice& c = m_choices.at(index);
    if (c.m_action)
        (*c.m_action)();
    return c.m_nextLine;
}

}

