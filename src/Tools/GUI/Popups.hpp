#pragma once

#include <imgui.h>
#include <deque>
#include <string>

namespace ImGui {

/**
 * @brief Manage informations popups with three levels : info, warning and error
 */
class Popups
{
public:
    static void Error(const std::string& message);
    static void Warning(const std::string &message);
    static void Info(const std::string& message);

    static void Draw();
private:
    static Popups instance;                     ///< Singleton instance
    /// @brief Default constructor
    Popups() = default;

    std::deque<std::string> m_errorList;        ///< Queue of error messages
    std::deque<std::string> m_warningList;      ///< Queue of warning messages
    std::deque<std::string> m_infoList;         ///< Queue of info messages
    bool m_popupOpen = false;                   ///< Is there a popup already open
};

} // namespace ImGui
