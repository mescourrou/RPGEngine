#include "ConsoleWindow.hpp"
#include <Logger.hpp>

namespace maker::gui
{

/**
 * @brief Prepare the window to be drawn
 * @return Return true if all went well
 */
bool ConsoleWindow::doPrepare()
{
    bool scrollDown = false;
    if (Logger::newLogs())
        scrollDown = true;
    ImGui::TextUnformatted(Logger::getLog().c_str());
    if (scrollDown)
        ImGui::SetScrollHereY(1.0F);
    return true;
}

} // namespace maker::gui

