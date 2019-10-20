#pragma once

#include <vector>
#include <Window.hpp>

namespace ImGui
{

/**
 * @brief Manager of Windows
 */
class WindowsManager
{
  public:
    /// @brief Default constructor
    WindowsManager() = default;

    void prepareWindows();
    /**
     * @brief Add a window to the list
     * @param w Pointer on the window to add
     */
    void addWindow(Window* w)
    {
        m_windowList.push_back(w);
    }

    /**
     * @brief Get the windows list
     */
    std::vector<Window*> windowsList() const
    {
        return m_windowList;
    }
  private:
    std::vector<Window*> m_windowList; ///< List of windows to print
};

} // namespace ImGui
