#include "InventoryWindow.hpp"
namespace game::GUI
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
bool game::GUI::InventoryWindow::doCollapsedState()
{
    ImGui::SetWindowCollapsed(false);
    setActive(false);
    return true;
}

} // namespace game::GUI
