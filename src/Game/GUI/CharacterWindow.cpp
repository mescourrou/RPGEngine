#include "CharacterWindow.hpp"

namespace game::gui
{

/**
 * @brief Prepare the window to be drawn
 * @return Return true if all went well
 */
bool CharacterWindow::doPrepare()
{
    ImGui::Text("Soon");

    return true;
}

/**
 * @brief Collapse button close the window
 */
bool CharacterWindow::doCollapsedState()
{
    ImGui::SetWindowCollapsed(false);
    setActive(false);
    return true;
}

} // namespace game::gui

