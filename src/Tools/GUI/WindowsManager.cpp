#include "WindowsManager.hpp"

namespace ImGui
{

/**
 * @brief Prepare the windows of the list
 */
void WindowsManager::prepareWindows()
{
    for (auto& w : m_windowList)
    {
        if (w)
            w->prepare();
    }
}

} // namespace ImGui

