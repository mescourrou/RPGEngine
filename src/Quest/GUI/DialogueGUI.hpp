#pragma once

// STL
#include <memory>

// Project
#include <BaseException.hpp>

// Thirdparty
#include <Window.hpp>
#include <imgui.h>

namespace quest
{
class Dialogue;
class DialogueLine;
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
    DialogueGUI(std::weak_ptr<const Dialogue> dialogue);
    ~DialogueGUI() override = default;

    void setPlayerName(const std::string& playerName)
    {
        m_playerName = playerName;
    }

  protected:
    bool doPrepare() override;
    bool doCollapsedState() override;
  private:
    ImVec4 m_colorCurrentLine;
    ImVec4 m_colorPlayer;
    std::string m_playerName;
    std::vector<std::pair<std::string, std::string>> m_previousLines;
    std::weak_ptr<const Dialogue> m_dialogue;
    std::weak_ptr<const DialogueLine> m_currentLine;
};

} // namespace gui

} // namespace character
