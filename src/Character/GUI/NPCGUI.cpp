#include "NPCGUI.hpp"
#include <NPC.hpp>

namespace character::gui
{

NPCGUI::NPCGUI(std::weak_ptr<NPC> character,
               std::shared_ptr<config::Context> context) :
    CharacterGUI(character, context)
{

}


} // namespace character::gui
