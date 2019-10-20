#pragma once

#include <imgui.h>
#include <string>

namespace ImGui
{

/**
 * @brief Handle all the elements around windows : activation, name, ...
 */
class Window
{
  public:
    Window(const std::string& title);
    /// @brief Destructor
    virtual ~Window() = default;

    virtual bool prepare() final;

    /**
     * @brief Set the window active or not
     * @param active Active state
     */
    virtual void setActive(bool active = true) final
    {
        m_active = active;
    }
    /**
     * @brief Get the current active status
     */
    virtual bool active() const final
    {
        return m_active;
    }
    /**
     * @brief Get the current active status, modifyable
     */
    virtual bool& active() final
    {
        return m_active;
    }
    /**
     * @brief Get the title of the window
     */
    virtual const std::string& title() const final
    {
        return m_title;
    }
    /**
     * @brief Set the title of the window
     */
    virtual void setTitle(const std::string& name) final
    {
        m_title = name;
    }

  protected:
    /**
     * @brief Hook for the inside of the window
     */
    virtual bool doPrepare() = 0;
    /**
     * @brief Hook when the window is collapsed
     */
    virtual bool doCollapsedState()
    {
        return true;
    }
  private:
    bool m_active = true;       ///< Window printed
    std::string m_title;         ///< Title of the window
};

} // namespace ImGui
