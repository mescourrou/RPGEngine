#include "DialogueGUI.hpp"

namespace quest::gui
{

DialogueGUI::DialogueGUI() : ImGui::Window("Dialogue")
{

}

DialogueGUI::DialogueGUI(const Dialogue* dialogue) :
    ImGui::Window("Dialogue"), m_dialogue(dialogue)
{

}

bool DialogueGUI::doPrepare()
{
    ImGui::Text("Soon");
    return true;
}

bool DialogueGUI::doCollapsedState()
{
    ImGui::SetWindowCollapsed(false);
    setActive(false);
    return true;
}


} // namespace quest::gui
