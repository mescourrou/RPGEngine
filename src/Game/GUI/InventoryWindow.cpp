#include "InventoryWindow.hpp"
namespace game::GUI {

bool InventoryWindow::doPrepare()
{
    ImGui::Text("Soon");
    return true;
}

bool game::GUI::InventoryWindow::doCollapsedState()
{
    ImGui::SetWindowCollapsed(false);
    setActive(false);
    return true;
}

} // namespace game::GUI
