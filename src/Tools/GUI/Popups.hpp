#pragma once

#include <imgui.h>
#include <deque>
#include <string>

namespace ImGui {

class Popups
{
public:
    static void Error(const std::string& message);
    static void Warning(const std::string &message);
    static void Info(const std::string& message);

    static void Draw();
private:
    static Popups instance;
    Popups();

    std::deque<std::string> m_errorList;
    std::deque<std::string> m_warningList;
    std::deque<std::string> m_infoList;
    bool m_popupOpen = false;
};

} // namespace ImGui
