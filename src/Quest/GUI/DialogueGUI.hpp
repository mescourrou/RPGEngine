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
    explicit DialogueGUI(std::weak_ptr<const Dialogue> dialogue);
    ~DialogueGUI() override = default;

    /**
     * @brief Specify the player name, the one which talks to NPCs.
     * @param playerName Name of the player.
     */
    void setPlayerName(const std::string& playerName)
    {
        m_playerName = playerName;
    }

  protected:
    bool doPrepare() override;
    bool doCollapsedState() override;
  private:
    /// Color of the last NPC line.
    ImVec4 m_colorCurrentLine;
    /// Color of the player lines.
    ImVec4 m_colorPlayer;
    /// Player name.
    std::string m_playerName;
    /// Dialogue history.
    std::vector<std::pair<std::string, std::string>> m_previousLines;
    /// Pointer on the dialogue.
    std::weak_ptr<const Dialogue> m_dialogue;
    /// Pointer on the current NPC line.
    std::weak_ptr<const DialogueLine> m_currentLine;
};

} // namespace gui

} // namespace character
