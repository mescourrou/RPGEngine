#pragma once

#include <memory>

#include <SFML/Graphics/Shape.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include <BaseException.hpp>
#include <Vector.hpp>
#include <Context.hpp>
#include <memory>

// External libs
#include <json.hpp>
using json = nlohmann::json;

namespace character {

namespace GUI {

CREATE_EXCEPTION_CLASS(CharacterGUI)

/**
 * @brief GUI part of the Map
 */
class CharacterGUI : public sf::Transformable, public sf::Drawable
{
public:
    CharacterGUI() = default;
    ~CharacterGUI() override;

    void setPosition(float x, float y);

protected:
    bool load(const std::string& name, const std::string& characterRessourcesDir);

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
private:
    std::map<unsigned int, sf::Sprite> m_sprites;
    std::vector<sf::Texture*> m_textures;
    std::map<std::string, std::vector<unsigned int>> m_actions;

    mutable sf::Sprite* m_currentSprite = nullptr;

    static constexpr char SPRITE_SETS[] = "spriteSets";
    static constexpr char FIRST_ID[] = "firstId";
    static constexpr char SET_FILE[] = "setFile";
    static constexpr char HEIGHT[] = "height";
    static constexpr char WIDTH[] = "width";
    static constexpr char SPRITE_HEIGHT[] = "spriteHeight";
    static constexpr char SPRITE_WIDTH[] = "spriteWidth";
    static constexpr char HORIZONTAL_SHIFT[] = "horizontalShift";
    static constexpr char VERTICAL_SHIFT[] = "verticalShift";
    static constexpr char BACKGROUND[] = "background";
    static constexpr char ORIGIN_X[] = "originX";
    static constexpr char ORIGIN_Y[] = "originY";
    static constexpr char ACTIONS[] = "actions";

    struct actions {
    static constexpr char DOWN_STOPPED[] = "downStopped";
    static constexpr char DOWN[] = "down";
    static constexpr char UP_STOPPED[] = "upStopped";
    static constexpr char UP[] = "up";
    static constexpr char RIGHT_STOPPED[] = "rightStopped";
    static constexpr char RIGHT[] = "right";
    static constexpr char LEFT_STOPPED[] = "leftStopped";
    static constexpr char LEFT[] = "left";
    };


};

} // namespace GUI

} // namespace character
