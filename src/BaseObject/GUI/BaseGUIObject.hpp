#pragma once

#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Drawable.hpp>

#include <BaseObject.hpp>

#include <memory>

namespace map
{
class Position;
namespace gui
{
class MapGUI;
}
} // namespace map

/**
 * @brief Base of all GUI objects
 */
class BaseGUIObject : public BaseObject, public sf::Transformable,
    public sf::Drawable
{
  public:
    /**
     * @brief Prepare the drawing
     */
    virtual void prepare(const sf::Vector2f& targetSize) = 0;

    /**
     * @brief Set the position of the GUIObject on the screen
     * @param pos Position in pixels
     */
    virtual void setOnScreenPosition(const sf::Vector2f& pos) = 0;

    /**
     * @brief Set the current MapGUI
     * @param map Current mapGUI
     */
    virtual void setCurrentMap(std::weak_ptr<map::gui::MapGUI> map) final
    {
        m_mapGUI = map;
    }

    /**
     * @brief Get the current map pointer
     */
    virtual std::weak_ptr<map::gui::MapGUI> currentMap() const final
    {
        return m_mapGUI;
    }

    /**
     * @brief Load the GUI Object
     * @param path Directory path where are stored the needed elements
     * @return
     */
    virtual bool load(const std::string& path) = 0;
  protected:
    /**
     * @brief Draw the GUI Object on the target with the given states
     * @param target Target to draw on
     * @param states States to use
     */
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override = 0;
  private:
    std::weak_ptr<map::gui::MapGUI> m_mapGUI;      ///< Current map
};

