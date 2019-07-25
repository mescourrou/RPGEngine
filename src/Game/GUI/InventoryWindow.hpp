#pragma once

#include <Window.hpp>

namespace game::GUI {

class InventoryWindow : public ImGui::Window
{
public:
    InventoryWindow() : Window("Inventory") {}

protected:
    bool doPrepare() override;
    bool doCollapsedState() override;
};

} // namespace game::GUI
