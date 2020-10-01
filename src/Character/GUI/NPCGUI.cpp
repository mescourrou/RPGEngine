#include "NPCGUI.hpp"
#include <NPC.hpp>

namespace character::gui
{

NPCGUI::NPCGUI(std::weak_ptr<NPC> character,
               std::shared_ptr<config::Context> context) :
    CharacterGUI(character, context)
{

}

void NPCGUI::onRightClick()
{
    LOG(INFO) << "NPCGUI::onRightClick";
    const auto& dialogues = std::static_pointer_cast<NPC>
                            (characterPtr().lock())->dialogues();
    VLOG(verbosityLevel::OBJECT_CREATION) << "Look for NPC " <<
                                          characterPtr().lock()->name() << " dialogues";
    if (dialogues.size() > 0)
    {
        m_dialogueWindow = std::make_unique<quest::gui::DialogueGUI>
                           (&dialogues.front());
        m_dialogueWindow->setTitle(characterPtr().lock()->name());
        m_dialogueWindow->setActive();
    }
}



} // namespace character::gui
