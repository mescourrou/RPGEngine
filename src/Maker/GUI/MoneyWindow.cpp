#include "MoneyWindow.hpp"
#include <glog/logging.h>
#include <Popups.hpp>

namespace maker::gui
{

/**
 * @brief Create a money window
 * @param maker Pointer on the maker backend
 */
MoneyWindow::MoneyWindow(Maker* maker) :
    Window("Money system"), m_maker(maker)
{

}

/**
 * @brief Prepare the window to be drawn
 * @return Return true if all went well
 */
bool MoneyWindow::doPrepare()
{
    if (m_maker->stateMachine.state() != Maker::States::WORKBENCH)
    {
        setActive(false);
        return false;
    }
    if (!m_moneyLoaded)
    {
        if (!m_maker->getMoneyInformations(m_infos))
            LOG(WARNING) << "Fail to load money information";
        m_nameList = m_infos.moneyList;
        m_moneyLoaded = true;
    }
    ImGui::Text("Base money : %s",
                (m_nameList.size() > 0 ? m_nameList.get(m_infos.baseMoney) : "None"));
    for (unsigned int i = 0; i < m_nameList.size() ; i++)
    {
        ImGui::PushID(i);
        ImGui::RadioButton("", &m_infos.baseMoney, i);
        ImGui::SameLine();
        ImGui::InputText("Name", m_nameList.get(i), 16,
                         ImGuiInputTextFlags_CharsNoBlank);

        ImGui::SameLine();
        if (m_infos.baseMoney != i)
        {
            if (ImGui::InputInt("Value", (int*)&m_infos.values.at(i))
                    && m_infos.values.at(i) <= 0)
                m_infos.values.at(i) = 1;
        }
        else
        {
            m_infos.values.at(i) = 1;
            ImGui::Text("Value = 1");
        }
        ImGui::PopID();
    }
    if (ImGui::Button("Save"))
    {
        doSaveMoney();
    }
    ImGui::SameLine();
    if (ImGui::Button("New money"))
    {
        m_nameList.push_back("");
        m_infos.values.push_back(1);
    }
    return true;
}

/**
 * @brief Prepare and call the backend method Maker::saveMoney
 */
void MoneyWindow::doSaveMoney()
{
    for (auto& v : m_infos.values)
    {
        if (std::count(m_infos.values.begin(), m_infos.values.end(), v) > 1)
        {
            ImGui::Popups::Error("Multiple money with same values (" + std::to_string(
                                     v) + ")");
            return;
        }
    }
    m_infos.moneyList = m_nameList.toVectorString();
    m_maker->saveMoney(m_infos);
}

} // namespace maker::gui

