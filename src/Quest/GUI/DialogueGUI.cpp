#include "DialogueGUI.hpp"
#include <Dialogue.hpp>
#include <DialogueLine.hpp>
#include <glog/logging.h>

namespace quest::gui
{

DialogueGUI::DialogueGUI() : ImGui::Window("Dialogue"), m_colorCurrentLine(255,
            255, 0, 255), m_colorPlayer(0, 255, 0, 255)
{

}

DialogueGUI::DialogueGUI(std::weak_ptr<const Dialogue> dialogue) :
    DialogueGUI()
{
    m_dialogue = dialogue;
    m_currentLine = m_dialogue.lock()->firstLine();
}

bool DialogueGUI::doPrepare()
{
    if (!m_dialogue.lock())
        return true;
    //    ImGui::BeginChild("Scrolling");
    ImGui::Columns(2);
    float width = 0;
    for (const auto& [characterName, line] : m_previousLines)
    {
        ImVec4 color(255, 255, 255, 255);
        if (characterName == m_playerName)
            color = m_colorPlayer;

        ImGui::TextColored(color, "%s", characterName.c_str());
        auto size = ImGui::CalcTextSize(characterName.c_str());
        float newWidth = size.x + size.y;
        if (newWidth > width)
            width = newWidth;
        ImGui::NextColumn();
        ImGui::TextColored(color, "%s", line.c_str());
        ImGui::NextColumn();
    }
    //    ImGui::EndChild();
    if (m_currentLine.lock())
    {
        ImGui::TextColored(m_colorCurrentLine, "%s",
                           m_dialogue.lock()->characterName().c_str());
        auto size = ImGui::CalcTextSize(m_dialogue.lock()->characterName().c_str());
        float newWidth = size.x + size.y;
        if (newWidth > width)
            width = newWidth;
        ImGui::NextColumn();
        ImGui::TextColored(m_colorCurrentLine, "%s",
                           m_currentLine.lock()->line().c_str());
        ImGui::SetColumnWidth(0, width);
        ImGui::Columns(1);
        if (m_currentLine.lock()->choices().size() == 1
                && m_currentLine.lock()->choices().front().empty())
        {
            m_previousLines.push_back({m_dialogue.lock()->characterName(), m_currentLine.lock()->line()});
            m_currentLine = m_currentLine.lock()->selectChoice(0);
        }
        else if (m_currentLine.lock()->choices().size() == 0)
        {
            m_previousLines.push_back({m_dialogue.lock()->characterName(), m_currentLine.lock()->line()});
            m_currentLine = {};
        }
        else
        {
            for (size_t i = 0; i < m_currentLine.lock()->choices().size(); i++)
            {
                if (m_currentLine.lock()->choices().at(i) !=
                        m_currentLine.lock()->choices().front())
                    ImGui::SameLine();
                if (ImGui::Button(m_currentLine.lock()->choices().at(i).c_str()))
                {
                    m_previousLines.push_back({m_dialogue.lock()->characterName(), m_currentLine.lock()->line()});
                    m_previousLines.push_back({m_playerName, m_currentLine.lock()->choices().at(i)});
                    m_currentLine = m_currentLine.lock()->selectChoice(i);
                }
            }
        }
    }

    return true;
}

bool DialogueGUI::doCollapsedState()
{
    ImGui::SetWindowCollapsed(false);
    setActive(false);
    return true;
}


} // namespace quest::gui
