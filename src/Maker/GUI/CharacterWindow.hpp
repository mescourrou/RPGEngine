#pragma once

#include <Window.hpp>
#include <Stringlist.hpp>
#include <memory>
#include <Maker.hpp>

namespace maker::gui
{

/**
 * @brief Character window
 */
class CharacterWindow : public ImGui::Window
{
  public:
    explicit CharacterWindow(Maker* maker);

  protected:
    bool doPrepare() override;

    void doNewCharacter();
    void doDeleteCharacter();
  private:
    /// Pointer on the maker backend
    Maker* m_maker;
    /// Character list
    stringlist m_list;
    /// Current character
    int m_currentCharacter = -1;
    /// Saved charater informations
    Maker::CharacterInformations m_current;
    /// Edited character informations
    Maker::CharacterInformations m_edit;
    /// Title of the window
    char m_title[16] = "";
    /// Is there a new character showed
    bool m_newOne = true;
};

} // namespace maker::gui
