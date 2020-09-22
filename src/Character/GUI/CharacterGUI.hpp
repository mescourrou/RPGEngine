#pragma once

#include <memory>
#include <chrono>

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

namespace game::gui
{
class GameGUI;
}

namespace character
{
class Character;
namespace gui
{

CREATE_EXCEPTION_CLASS(CharacterGUI)

/**
 * @brief GUI class related to the Character
 */
class CharacterGUI : public BaseGUIObject
{
    DECLARE_BASEOBJECT(CharacterGUI)
  public:
    static void connectSignals(game::gui::GameGUI* game, CharacterGUI* character,
                               bool player = false);
    static void connectSignals(Character*, CharacterGUI*,
                               bool = false);

    CharacterGUI(std::weak_ptr<Character> character,
                 std::shared_ptr<config::Context> context);
    /// @brief Default destructor
    ~CharacterGUI() override = default;

    void prepare(const sf::Vector2f&) override;

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
    static bool verifyJSONTopStructure(const json& json);
    static bool verifyJSONSpriteSetsStructure(const json& set);
    bool loadSets(const json& json, const std::string& characterRessourcesDir);
    bool loadActions(const json& json);

    std::weak_ptr<Character> m_character; ///< Pointer on the backend character
    std::shared_ptr<config::Context> m_context;

    std::map<unsigned int, sf::Sprite>
    m_sprites; ///< Sprites of the Character, assigned by id
    std::vector<std::shared_ptr<sf::Texture>>
                                           m_textures; ///< List of the textures to keep the ownership
    std::map<std::string, std::vector<unsigned int>>
            m_actions; ///< List of sprites associated with the actions

    sf::Sprite* m_currentSprite = nullptr; ///< Current sprite pointer
    unsigned int m_spriteCinematicIndex =
        0; ///< Current index on the curren^t action sprite list

    unsigned int m_spritePeriod; ///< Sprite refresh period (millisecond)
    std::chrono::time_point<std::chrono::system_clock> m_spriteTimer;

    Direction m_currentDirection = Down; ///< Current player direction
    bool m_moving = false; ///< Is the player moving ?

    std::vector<std::string>
    m_requiredActions; ///< Actions wich need to be found in the loaded file

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

} // namespace gui

} // namespace character
