#include "WindowsManager.hpp"
#include <InstrumentationTimer.hpp>

namespace ImGui
{

/**
 * @brief Prepare the windows of the list
 */
void WindowsManager::prepareWindows()
{
    PROFILE_FUNCTION();
    for (auto& w : m_windowList)
    {
        if (w)
            w->prepare();
    }
}

} // namespace ImGui

