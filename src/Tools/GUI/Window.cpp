#include "Window.hpp"

namespace ImGui
{

/**
 * @brief Constructor
 * @param title Title of the window
 */
Window::Window(const std::string& title) :
    m_title(title)
{

}

/**
 * @brief Prepare the window
 * @return Return true if all went well
 */
bool Window::prepare()
{
    if (!m_active)
        return false;
    if (ImGui::Begin(m_title.c_str()))
    {
        doPrepare();
        ImGui::End();
        return true;
    }
    else
        doCollapsedState();
    ImGui::End();
    return false;
}

} // namespace ImGui
