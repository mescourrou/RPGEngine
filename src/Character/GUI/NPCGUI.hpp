#pragma once

#include <memory>

#include "CharacterGUI.hpp"
#include <Context.hpp>
#include <DialogueGUI.hpp>

namespace character
{
class NPC;
namespace gui
{


class NPCGUI : public CharacterGUI
{
  public:
    NPCGUI(std::weak_ptr<NPC> character, std::shared_ptr<config::Context> context);
    ~NPCGUI() override = default;

    bool load(const std::string& characterRessourcesDir) override;
  protected:
    void onRightClick() override;
  private:
    std::unique_ptr<quest::gui::DialogueGUI> m_dialogueWindow;
};


}
} // namespace character::gui