#pragma once

#include <Window.hpp>

namespace game::gui
{

/**
 * @brief Inventory window
 */
class InventoryWindow : public ImGui::Window
{
  public:
    /// @brief Constructor
    InventoryWindow() : Window("Inventory") {}

  protected:
    bool doPrepare() override;
    bool doCollapsedState() override;
};

} // namespace game::gui
