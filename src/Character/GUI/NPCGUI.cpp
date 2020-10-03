#include "NPCGUI.hpp"
#include <NPC.hpp>
#include <Game.hpp>
#include <GameGUI.hpp>

namespace character::gui
{

/**
 * @brief Create a NPC GUI.
 * @param character Pointer on the NPC object attached.
 * @param context Global context.
 */
NPCGUI::NPCGUI(std::weak_ptr<NPC> character,
               std::shared_ptr<config::Context> context) :
    CharacterGUI(character, context)
{

}

/**
 * @brief Load the NPC and the Character from the file (snakeCase form of the name)
 * @param characterRessourcesDir Directory where are stored the characters files and sprites
 * @return Return true if the load was successfull
 */
bool NPCGUI::load(const std::string& characterRessourcesDir)
{
    const auto& dialogues = std::static_pointer_cast<NPC>
                            (characterPtr().lock())->dialogues();
    VLOG(verbosityLevel::OBJECT_CREATION) << "Look for NPC " <<
                                          characterPtr().lock()->name() << " dialogues";
    if (!dialogues.empty())
    {
        m_dialogueWindow = std::make_unique<quest::gui::DialogueGUI>(dialogues.front());
        m_dialogueWindow->setTitle(characterPtr().lock()->name());
        m_dialogueWindow->setActive(false);
        m_context->currentGame()->GUI().lock()->addImguiWindow(
            m_dialogueWindow.get());
    }
    return CharacterGUI::load(characterRessourcesDir);
}

/**
 * @brief Action on right click on the NPC.
 */
void NPCGUI::onRightClick()
{
    if (m_dialogueWindow)
    {
        m_dialogueWindow->setPlayerName(
            m_context->currentGame()->playerCharacter().lock()->name());
        m_dialogueWindow->setActive();
    }
    CharacterGUI::onRightClick();
}



} // namespace character::gui
