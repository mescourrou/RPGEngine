#include "CharacterWindow.hpp"

namespace game::GUI {

bool CharacterWindow::doPrepare()
{
    ImGui::Text("Soon");

    return true;
}

bool CharacterWindow::doCollapsedState()
{
    ImGui::SetWindowCollapsed(false);
    setActive(false);
    return true;
}

} // namespace game::GUI

