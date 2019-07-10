#pragma once

#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Drawable.hpp>

namespace map {
class Position;
} // namespace map

/**
 * @brief Base GUI interface
 */
class BaseGUIObject : public sf::Transformable, public sf::Drawable
{
public:
    virtual void prepare() = 0;
protected:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override = 0;
};

