#include "ConsoleWindow.hpp"
#include <Logger.hpp>

namespace maker::GUI {


bool ConsoleWindow::doPrepare()
{
    bool scrollDown = false;
    if (Logger::newLogs())
        scrollDown = true;
    ImGui::TextUnformatted(Logger::getLog().c_str());
    if (scrollDown)
        ImGui::SetScrollHereY(1.0f);
    return true;
}

} // namespace maker::GUI

