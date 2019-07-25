#pragma once

#include <imgui.h>
#include <string>

namespace ImGui {

class Window
{
public:
    Window(const std::string& name);
    virtual ~Window() = default;

    virtual bool prepare() final;

    virtual void open() final { m_open = true; }
    virtual void close() final { m_open = true; }
    virtual void setActive(bool active = true) final { m_active = active; }
    virtual bool active() const final { return m_active; }
    virtual bool& active() final { return m_active; }
    virtual const std::string& name() const final { return m_name; }
    virtual void setName(const std::string& name) final { m_name = name; }

protected:
    virtual bool doPrepare() = 0;
    virtual bool doCollapsedState() { return true; }
    bool m_open = false;
private:
    bool m_active = true;
    std::string m_name;
};

} // namespace ImGui
