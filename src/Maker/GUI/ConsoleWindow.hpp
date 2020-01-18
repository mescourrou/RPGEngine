#pragma once

#include <Window.hpp>

namespace maker::gui
{

/**
 * @brief Console window
 */
class ConsoleWindow : public ImGui::Window
{
  public:
    ConsoleWindow() : Window("Console") {}

  protected:
    bool doPrepare() override;
};

} // namespace maker::gui
