#pragma once

#include <memory>

#include <SFML/Graphics/Shape.hpp>

#include <BaseException.hpp>
#include <Vector.hpp>
#include <Map.hpp>

namespace map {

namespace GUI {

CREATE_EXCEPTION_CLASS(MapGUI)

class MapGUI : public sf::Transformable, public sf::Drawable
{
public:
    MapGUI(std::shared_ptr<Map> map);
    ~MapGUI() override final = default;


protected:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

private:
    Vector<3> m_centerOfView;
    float m_zoom = 1;

    std::shared_ptr<Map> m_map;


};

} // namespace GUI

} // namespace map
