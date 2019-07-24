#pragma once

#include <Window.hpp>
#include <Maker.hpp>
#include <Stringlist.hpp>

namespace maker::GUI {

class MoneyWindow : public ImGui::Window
{
public:
    MoneyWindow(Maker* maker);

protected:
    bool doPrepare() override;

    void doSaveMoney();

    Maker* m_maker;

    Maker::MoneyInformations m_infos;
    stringlist m_nameList;
    bool m_moneyLoaded = false;
};

} // namespace maker::GUI
