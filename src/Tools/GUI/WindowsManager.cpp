#include "WindowsManager.hpp"

namespace ImGui {

void WindowsManager::prepareWindows()
{
    for (auto& w : m_windowList)
    {
        if (w)
            w->prepare();
    }
}

} // namespace ImGui

