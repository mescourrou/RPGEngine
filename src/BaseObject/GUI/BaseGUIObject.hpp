#pragma once

#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Drawable.hpp>

#include <BaseObject.hpp>

#include <memory>

namespace map {
class Position;
namespace GUI {
class MapGUI;
}
} // namespace map

/**
 * @brief Base GUI interface
 */
class BaseGUIObject : public BaseObject, public sf::Transformable, public sf::Drawable
{
public:
    /**
     * @brief Prepare the drawing
     */
    virtual void prepare(const sf::Vector2u& targetSize) = 0;
    virtual void setOnScreenPosition(const sf::Vector2f& pos) = 0;
    virtual void setCurrentMap(std::weak_ptr<map::GUI::MapGUI> map) final { m_map = map; }
protected:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override = 0;
    std::weak_ptr<map::GUI::MapGUI> m_map;
};

