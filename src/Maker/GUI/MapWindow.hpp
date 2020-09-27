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
    /// Pointer on the maker
    Maker* m_maker;

    /// Editted informations
    Maker::MapInformations m_edit;
    /// Currently saved informations
    Maker::MapInformations m_current;
    /// List of maps
    stringlist m_list;
    /// Selected map
    unsigned int m_selected = 0;
    /// Is the map loaded
    bool m_loaded = false;
    /// Is there a new map
    bool m_newOne = false;
};

} // namespace maker::gui
