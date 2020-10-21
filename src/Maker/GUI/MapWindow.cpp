#include "MapWindow.hpp"
#include <Tools.hpp>

namespace maker::gui
{

/**
 * @brief Create a MapWindow
 * @param maker Pointer on the maker backend
 */
MapWindow::MapWindow(Maker* maker) :
    Window("Map"), m_maker(maker)
{

}

/**
 * @brief Prepare the window to be drawn
 * @return Return true if all went well
 */
bool MapWindow::doPrepare()
{
    if (m_maker->getStateMachine().state() != Maker::States::WORKBENCH)
    {
        setActive(false);
        return false;
    }
    if (!m_loaded)
    {
        m_list = m_maker->getMapList();
        if (m_list.size() > 0)
            m_maker->setCurrentMap(m_list.getStr(0));
        else
            m_newOne = true;
        m_loaded = true;
    }
    ImGui::Columns(2);
    for (unsigned int i = 0; i < m_list.size(); i++)
    {
        ImGui::PushID(i);
        if (ImGui::RadioButton("##push", (int*)&m_selected, i))
        {
            m_maker->setCurrentMap(m_list.getStr(i));
            m_edit = m_current = m_maker->getMapInformations(m_list.getStr(i));
        }
        ImGui::SameLine();
        ImGui::Text("%s", m_list.get(i));
        ImGui::PopID();
    }
    if (ImGui::Button("New map"))
    {
        m_list.push_back("New map");
        m_selected = m_list.size() - 1;
        m_newOne = true;
    }
    ImGui::SameLine();
    if (ImGui::Button("Delete map"))
    {
        m_list.remove(m_selected);
        if (m_selected > 0)
            m_selected--;

    }
    ImGui::NextColumn();
    if (m_list.size())
    {
        ImGui::InputText("Name", m_list.get(m_selected), 16);
        ImGui::Text("Create a file named '%s.json'\nwith Tiled in the map directory",
                    Tools::snakeCase(m_list.getStr(m_selected)).c_str());
        if (ImGui::Button("Save"))
        {
            m_maker->setCurrentMap(m_list.getStr(m_selected));
            m_edit.name = m_list.getStr(m_selected);
            if (m_newOne)
                m_maker->saveMap(m_edit);
            else
                m_maker->saveMap(m_edit, m_current);
        }
    }
    return true;
}

} // namespace maker::gui


