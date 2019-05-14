#include "MapGUI.hpp"

namespace map {

namespace GUI {

map::GUI::MapGUI::MapGUI(std::shared_ptr<map::Map> map) :
    m_map(map)
{

}

void map::GUI::MapGUI::draw(sf::RenderTarget &target, sf::RenderStates states) const
{

}


} // namespace GUI

} // namespace map
