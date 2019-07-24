#include "CharacterWindow.hpp"
#include <Popups.hpp>

namespace maker::GUI {

CharacterWindow::CharacterWindow(Maker *maker) :
    Window("Character"), m_maker(maker)
{
    m_maker->signalCharacterListUpdated.subscribeAsync([this](std::vector<std::string> list){
        m_list = list;
        open();
    });
}

bool CharacterWindow::doPrepare()
{
    ImGui::Columns(2);
    if (ImGui::ListBox("", &m_currentCharacter, m_list.data(), m_list.size()))
    {
        if (!m_maker->getCharacterInformations(m_list.getStr(m_currentCharacter), m_current))
        {
            ImGui::Popups::Error(std::string("Error while loading " + m_list.getStr(m_currentCharacter) + " character informations").c_str());
            doDeleteCharacter();
            doNewCharacter();
        }
        else
        {
            m_edit = m_current;
            strcpy(m_name, m_edit.name.c_str());
            m_newOne = false;
        }
    }

    ImGui::NextColumn();
    ImGui::InputText("Name", m_name, 16);
    ImGui::RadioButton("NPC", (int*)&m_edit.type, Maker::CharacterInformations::NPC);
    ImGui::RadioButton("Vendor", (int*)&m_edit.type, Maker::CharacterInformations::VENDOR);

    if (strlen(m_name) > 0)
    {
        if (ImGui::Button("Save"))
        {
            m_edit.name = m_name;
            if (m_newOne)
            {
                m_maker->saveCharacter(m_edit);
                m_newOne = false;
            }
            else
                m_maker->saveCharacter(m_edit, m_current);
            m_current = m_edit;

        }
    }
    ImGui::Columns();
    if (ImGui::Button("New"))
    {
        doNewCharacter();
    }
    ImGui::SameLine();
    if (ImGui::Button("Delete"))
    {
        doDeleteCharacter();
    }
    return true;
}


void CharacterWindow::doNewCharacter()
{
    m_newOne = true;
    m_edit = m_current = Maker::CharacterInformations();
    strcpy(m_name, "");
    m_currentCharacter = -1;
}

void CharacterWindow::doDeleteCharacter()
{
    if (!m_current.name.empty())
        m_maker->deleteCharacter(m_current.name);
    doNewCharacter();
}


} // namespace maker::GUI
