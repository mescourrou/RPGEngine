#pragma once

#include <Window.hpp>
#include <Maker.hpp>
#include <Stringlist.hpp>

namespace maker::GUI {

/**
 * @brief Money settings window
 */
class MoneyWindow : public ImGui::Window
{
public:
    MoneyWindow(Maker* maker);

protected:
    bool doPrepare() override;

    void doSaveMoney();

    Maker* m_maker;                     ///< Pointer on the maker backend

    Maker::MoneyInformations m_infos;   ///< Money informations needed for the backend
    stringlist m_nameList;              ///< List of the money names
    bool m_moneyLoaded = false;         ///< Is money loaded?
};

} // namespace maker::GUI
