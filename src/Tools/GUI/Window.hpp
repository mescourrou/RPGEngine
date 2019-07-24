#pragma once

#include <imgui.h>
#include <string>

namespace ImGui {

class Window
{
public:
    Window(const std::string& name);

    virtual bool prepare() final;

    virtual void open() final { m_open = true; }
    virtual void close() final { m_open = true; }
    virtual void setActive(bool active = true) final { m_active = active; }
    virtual bool active() const final { return m_active; }
    virtual bool& active() final { return m_active; }
    virtual const std::string& name() const final { return m_name; }

protected:
    virtual bool doPrepare() = 0;
    bool m_open = false;
private:
    std::string m_name;
    bool m_active = true;
};

} // namespace ImGui
