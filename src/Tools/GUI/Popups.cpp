#include "Popups.hpp"

namespace ImGui {

Popups Popups::instance;

/**
 * @brief Add an error message to be printed
 * @param message Message to add
 */
void Popups::Error(const std::string& message)
{
    instance.m_errorList.push_back(message);
}

/**
 * @brief Add a warning message to be printed
 * @param message Message to add
 */
void Popups::Warning(const std::string& message)
{
    instance.m_warningList.push_back(message);
}

/**
 * @brief Add an information message
 * @param message Message to add
 */
void Popups::Info(const std::string &message)
{
    instance.m_infoList.push_back(message);
}

/**
 * @brief Draws the current popup or open the one with the higher level
 */
void Popups::Draw()
{
    if (!instance.m_popupOpen)
    {
        if (!instance.m_errorList.empty())
        {
            ImGui::OpenPopup("Error");
            instance.m_popupOpen = true;
        }
        else if (!instance.m_warningList.empty())
        {
            ImGui::OpenPopup("Warning");
            instance.m_popupOpen = true;
        }
        else if (!instance.m_infoList.empty())
        {
            ImGui::OpenPopup("Informations");
            instance.m_popupOpen = true;
        }
    }

    if (instance.m_popupOpen)
    {
        if (ImGui::BeginPopupModal("Error"))
        {
            ImGui::Text(instance.m_errorList.front().c_str());

            if (ImGui::Button("OK"))
            {
                instance.m_errorList.pop_front();
                instance.m_popupOpen = false;
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
        if (ImGui::BeginPopupModal("Warning"))
        {
            ImGui::Text(instance.m_warningList.front().c_str());

            if (ImGui::Button("OK"))
            {
                instance.m_warningList.pop_front();
                instance.m_popupOpen = false;
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
        if (ImGui::BeginPopup("Informations"))
        {
            ImGui::Text(instance.m_infoList.front().c_str());

            if (ImGui::Button("OK"))
            {
                instance.m_infoList.pop_front();
                instance.m_popupOpen = false;
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
    }
}

} // namespace ImGui
