#pragma once

// STL
#include <memory>

// Project
#include <BaseException.hpp>

// Thirdparty
#include <Window.hpp>

namespace quest
{
class Dialogue;
namespace gui
{

CREATE_EXCEPTION_CLASS(DialogueGUI)

/**
 * @brief GUI class related to the Character
 */
class DialogueGUI : public ImGui::Window
{
  public:
    DialogueGUI();
    DialogueGUI(const Dialogue* dialogue);
    ~DialogueGUI() override = default;

  protected:
    bool doPrepare() override;
    bool doCollapsedState() override;
  private:

    const Dialogue* m_dialogue = nullptr;
};

} // namespace gui

} // namespace character
