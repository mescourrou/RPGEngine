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
    CharacterGUI();
    /// @brief Default destructor
    ~CharacterGUI() override = default;

    void setPosition(float x, float y);

    void doSubscribeKeyEvents(game::GUI::GameGUI* game);

    void prepare() override;

protected:
    bool load(const std::string& name, const std::string& characterRessourcesDir);
    void eventKeyPressed(sf::Event::KeyEvent);
    void eventKeyReleased(sf::Event::KeyEvent key);

    /**
     * @brief Direction of the character
     */
    enum Direction {
        Up, Down, Left, Right
    };
    /**
     * @brief Hook to process the instruction to move the character
     * @param dir Direction to move the character to
     */
    virtual void doMove(Direction dir) = 0;

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
private:
    std::map<unsigned int, sf::Sprite> m_sprites;               ///< Sprites of the Character, assigned by id
    std::vector<std::shared_ptr<sf::Texture>> m_textures;       ///< List of the textures to keep the ownership
    std::map<std::string, std::vector<unsigned int>> m_actions; ///< List of sprites associated with the actions

    mutable sf::Sprite* m_currentSprite = nullptr;              ///< Current sprite pointer
    unsigned int m_spriteCinematicIndex = 0;                    ///< Current index on the current action sprite list

    unsigned int m_spriteChangeTics = 5;                        ///< Number of image refreshing before changing the sprite
    unsigned int m_tics = 0;                                    ///< Image counter, see m_spriteChangeTics

    Direction m_currentDirection = Down;                        ///< Current player direction
    bool m_moving = false;                                      ///< Is the player moving ?

    std::vector<std::string> m_requiredActions;                 ///< Actions wich need to be found in the loaded file

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

    /**
     * @brief Differents actions names
     */
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
