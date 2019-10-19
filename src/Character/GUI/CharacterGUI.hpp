#pragma once

#include <memory>

#include <BaseException.hpp>
#include <Vector.hpp>
#include <Context.hpp>
#include <memory>
#include <Position.hpp>
#include <BaseGUIObject.hpp>

// External libs
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <json.hpp>
using json = nlohmann::json;

namespace game::GUI
{
class GameGUI;
}

namespace character
{
class Character;
namespace GUI
{

CREATE_EXCEPTION_CLASS(CharacterGUI)

/**
 * @brief GUI class related to the Character
 */
class CharacterGUI : public BaseGUIObject
{
    DECLARE_BASEOBJECT(CharacterGUI)
  public:
    static void connectSignals(game::GUI::GameGUI* game, CharacterGUI* character,
                               bool player = false);
    static void connectSignals(Character* character, CharacterGUI* characterGUI,
                               bool player = false);

    CharacterGUI(std::weak_ptr<Character> character,
                 std::shared_ptr<config::Context> context);
    /// @brief Default destructor
    ~CharacterGUI() override = default;

    void prepare(const sf::Vector2f& targetSize) override;

    void setOnScreenPosition(const sf::Vector2f& position) override;

    bool load(const std::string& characterRessourcesDir) override;

    void slotArrowPressed(sf::Keyboard::Key arrow);
    void slotKeyReleased(sf::Event::KeyEvent key);

    void uiRealtimeInformations();
    void uiFullInformations();
    void uiInventoryWindow();
  protected:


    /**
     * @brief Direction of the character
     */
    enum Direction
    {
        Up, Down, Left, Right
    };

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
  private:
    std::weak_ptr<Character>
    m_character;                       ///< Pointer on the backend character
    std::shared_ptr<config::Context> m_context;

    std::map<unsigned int, sf::Sprite>
    m_sprites;               ///< Sprites of the Character, assigned by id
    std::vector<std::shared_ptr<sf::Texture>>
                                           m_textures;       ///< List of the textures to keep the ownership
    std::map<std::string, std::vector<unsigned int>>
            m_actions; ///< List of sprites associated with the actions

    sf::Sprite* m_currentSprite =
        nullptr;                      ///< Current sprite pointer
    unsigned int m_spriteCinematicIndex =
        0;                    ///< Current index on the current action sprite list

    unsigned int
    m_spriteChangeTics;                            ///< Number of image refreshing before changing the sprite
    unsigned int m_tics =
        0;                                    ///< Image counter, see m_spriteChangeTics

    Direction m_currentDirection =
        Down;                        ///< Current player direction
    bool m_moving =
        false;                                      ///< Is the player moving ?

    std::vector<std::string>
    m_requiredActions;                 ///< Actions wich need to be found in the loaded file

    /**
     * @brief Differents actions names
     */
    struct actions
    {
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
