#pragma once

#include <memory>
#include <vector>
#include <Window.hpp>
#include <Stringlist.hpp>

namespace ImGui {

class WindowsManager
{
public:
    WindowsManager() = default;

    void prepareWindows();
    void addWindow(Window* w) { m_windowList.push_back(w); }

    std::vector<Window*> windowsList() const { return m_windowList; }
private:
    std::vector<Window*> m_windowList;
};

} // namespace ImGui
