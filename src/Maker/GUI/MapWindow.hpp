#pragma once

#include <Window.hpp>
#include <Stringlist.hpp>
#include <Maker.hpp>

namespace maker::gui
{

/**
 * @brief Map window
 */
class MapWindow : public ImGui::Window
{
  public:
    explicit MapWindow(Maker* maker);

  protected:
    bool doPrepare() override;
  private:
    Maker* m_maker;                     ///< Pointer on the maker

    Maker::MapInformations m_edit;      ///< Editted informations
    Maker::MapInformations m_current;   ///< Currently saved informations
    stringlist m_list;                  ///< List of maps
    unsigned int m_selected = 0;        ///< Selected map
    bool m_loaded = false;              ///< Is the map loaded
    bool m_newOne = false;              ///< Is there a new map
};

} // namespace maker::gui
