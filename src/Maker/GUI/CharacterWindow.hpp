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
    Maker* m_maker;                             ///< Pointer on the maker backend
    stringlist m_list;                          ///< Character list
    int m_currentCharacter = -1;                ///< Current character
    Maker::CharacterInformations m_current;     ///< Saved charater informations
    Maker::CharacterInformations m_edit;        ///< Edited character informations
    char m_title[16] = "";                      ///< Title of the window
    bool m_newOne = true;                       ///< Is there a new character showed
};

} // namespace maker::gui
