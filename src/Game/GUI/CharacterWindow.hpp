#pragma once

#include <Window.hpp>
namespace game::GUI {

class CharacterWindow : public ImGui::Window
{
public:
    CharacterWindow() : Window("Character") {}

protected:
    bool doPrepare() override;
    bool doCollapsedState() override;
};

} // namespace game::GUI
