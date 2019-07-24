#pragma once

#include <Window.hpp>
#include <Stringlist.hpp>
#include <memory>
#include <Maker.hpp>

namespace maker::GUI {

class CharacterWindow : public ImGui::Window
{
public:
    CharacterWindow(Maker* maker);

protected:
    bool doPrepare() override;

    void doNewCharacter();
    void doDeleteCharacter();

    Maker* m_maker;
    stringlist m_list;
    int m_currentCharacter = -1;
    Maker::CharacterInformations m_current;
    Maker::CharacterInformations m_edit;
    char m_name[16] = "";
    bool m_newOne = true;
};

} // namespace maker::GUI
