#pragma once

#include <memory>

#include <BaseException.hpp>
#include <Vector.hpp>
#include <Context.hpp>
#include <memory>
#include <BaseGUIObject.hpp>

// External libs
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <json.hpp>
using json = nlohmann::json;

namespace game::GUI {
class GameGUI;
}

namespace character {

namespace GUI {

CREATE_EXCEPTION_CLASS(CharacterGUI)

/**
 * @brief GUI interface for Character
 */
class CharacterGUI : public BaseGUIObject
{
public:
    CharacterGUI() = default;
    ~CharacterGUI() override;

    void setPosition(float x, float y);

    void doSubscribeKeyEvents(game::GUI::GameGUI* game);

    void prepare() override;

protected:
    bool load(const std::string& name, const std::string& characterRessourcesDir);
    void eventKeyPressed(sf::Event::KeyEvent);
    void eventKeyReleased(sf::Event::KeyEvent key);

    enum Direction {
        Up, Down, Left, Right
    };
    Direction m_currentDirection = Down;
    bool m_moving = false;
    virtual void doMove(Direction dir) = 0;

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
private:
    std::map<unsigned int, sf::Sprite> m_sprites;
    std::vector<sf::Texture*> m_textures;
    std::map<std::string, std::vector<unsigned int>> m_actions;

    mutable sf::Sprite* m_currentSprite = nullptr;
    unsigned int m_spriteCinematicIndex = 0;

    unsigned int m_spriteChangeTics = 5;
    unsigned int m_tics = 0;

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
