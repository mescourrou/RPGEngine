#include "DialogueWindow.hpp"

namespace maker::gui
{

DialogueWindow::DialogueWindow(Maker* maker) :
    Window("Dialogue"), m_maker(maker)
{

}

bool DialogueWindow::doPrepare()
{
    ImGui::Text("Soon");
    return true;
}

} // namespace maker::gui
