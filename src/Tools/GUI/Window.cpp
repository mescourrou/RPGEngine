#include "Window.hpp"

namespace ImGui {

Window::Window(const std::string &name) :
    m_name(name)
{

}

bool Window::prepare()
{
    if (!m_active || !m_open)
        return false;
    if (ImGui::Begin(m_name.c_str()))
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
