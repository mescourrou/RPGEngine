#pragma once

#include <Window.hpp>
#include <Maker.hpp>
#include <Stringlist.hpp>

namespace maker::gui
{

/**
 * @brief Money settings window
 */
class MoneyWindow : public ImGui::Window
{
  public:
    explicit MoneyWindow(Maker* maker);

  protected:
    bool doPrepare() override;

    void doSaveMoney();
  private:
    /// Pointer on the maker backend
    Maker* m_maker;

    /// Money informations needed for the backend
    Maker::MoneyInformations m_infos;
    /// List of the money names
    stringlist m_nameList;
    /// Is money loaded?
    bool m_moneyLoaded = false;
};

} // namespace maker::gui
