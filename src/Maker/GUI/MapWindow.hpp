#pragma once

#include <Window.hpp>
#include <Stringlist.hpp>
#include <Maker.hpp>

namespace maker::GUI {

class MapWindow : public ImGui::Window
{
public:
    MapWindow(Maker* maker);

protected:
    bool doPrepare() override;

    Maker* m_maker;

    Maker::MapInformations m_edit;
    Maker::MapInformations m_current;
    stringlist m_list;
    unsigned int m_selected = 0;
    bool m_loaded = false;
    bool m_newOne = false;
};

} // namespace maker::GUI
