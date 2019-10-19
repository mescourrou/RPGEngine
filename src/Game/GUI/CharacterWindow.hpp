#pragma once

#include <Window.hpp>
namespace game::GUI
{

/**
 * @brief Character windown
 */
class CharacterWindow : public ImGui::Window
{
  public:
    /**
     * @brief Constructor
     */
    CharacterWindow() : Window("Character") {}

  protected:
    bool doPrepare() override;
    bool doCollapsedState() override;
};

} // namespace game::GUI
