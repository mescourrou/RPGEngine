#include "InventoryWindow.hpp"
namespace game::gui
{

/**
 * @brief Prepare the window to be drawn
 * @return Return true if all went well
 */
bool InventoryWindow::doPrepare()
{
    ImGui::Text("Soon");
    return true;
}

/**
 * @brief Collapse button close the window
 */
bool game::gui::InventoryWindow::doCollapsedState()
{
    ImGui::SetWindowCollapsed(false);
    setActive(false);
    return true;
}

} // namespace game::gui
