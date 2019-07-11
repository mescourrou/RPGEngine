#include "CharacterGUI.hpp"

// Stl
#include <fstream>
#ifdef BUILD_USE_FILESYSTEM
#include <filesystem>
#endif

// Project
#include <Tools.hpp>
#include <GameGUI.hpp>

// External libs
#include <SFML/Graphics/Shape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

namespace character::GUI {

/**
 * @brief Constructor
 */
CharacterGUI::CharacterGUI() :
    m_requiredActions({actions::UP, actions::DOWN, actions::LEFT, actions::RIGHT,
                             actions::UP_STOPPED, actions::DOWN_STOPPED, actions::LEFT_STOPPED, actions::RIGHT_STOPPED})
{

}

/**
 * @brief Subscribe on the game key events
 * @param game Game hosting the events
 */
void CharacterGUI::doSubscribeKeyEvents(game::GUI::GameGUI* game)
{
    if (game)
        game->signalKeyReleased.subscribeSync(this, &CharacterGUI::eventKeyReleased);
}

/**
 * @brief Implementation of BaseGUIObject::prepare : change the sprite according to the current state (moving and direction)
 */
void CharacterGUI::prepare()
{
    auto actualiseCurrentSprite = [this](const std::vector<unsigned int>& action){
        m_currentSprite = &(m_sprites[action.at(m_spriteCinematicIndex)]);
        m_spriteCinematicIndex++;
        if (m_spriteCinematicIndex >= action.size())
            m_spriteCinematicIndex = 0;
    };
    if (m_moving && m_currentDirection == Left)
    {
        if (m_tics == 0)
            actualiseCurrentSprite(m_actions[actions::LEFT]);
        doMove(Left);
    }
    else if (m_moving && m_currentDirection == Right)
    {
        if (m_tics == 0)
            actualiseCurrentSprite(m_actions[actions::RIGHT]);
        doMove(Right);
    }
    else if (m_moving && m_currentDirection == Down)
    {
        if (m_tics == 0)
            actualiseCurrentSprite(m_actions[actions::DOWN]);
        doMove(Down);
    }
    else if (m_moving && m_currentDirection == Up)
    {
        if (m_tics == 0)
            actualiseCurrentSprite(m_actions[actions::UP]);
        doMove(Up);
    }
    // If not moving, we cycle the stopped sprites
    if (!m_moving)
    {
        switch (m_currentDirection) {
        case Left:
        if (m_tics == 0)
            actualiseCurrentSprite(m_actions[actions::LEFT_STOPPED]);
            break;
        case Right:
        if (m_tics == 0)
            actualiseCurrentSprite(m_actions[actions::RIGHT_STOPPED]);
            break;
        case Up:
        if (m_tics == 0)
            actualiseCurrentSprite(m_actions[actions::UP_STOPPED]);
            break;
        case Down:
        if (m_tics == 0)
            actualiseCurrentSprite(m_actions[actions::DOWN_STOPPED]);
            break;
        }
    }
    m_tics++;
    if (m_tics >= m_spriteChangeTics)
        m_tics = 0;
}

/**
 * @brief Set the position of the current sprite on the screen
 * @param position Position of the sprite
 */
void CharacterGUI::setPositionOnScreen(const sf::Vector2f &position)
{
    m_currentSprite->setPosition(position);
}

/**
 * @brief Verify the key pressed on the keyboard and prepare the actions
 */
void CharacterGUI::watchKeyboard()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && (!m_moving || m_currentDirection == Left))
    {
        m_moving = true;
        m_currentDirection = Left;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && (!m_moving || m_currentDirection == Right))
    {
        m_moving = true;
        m_currentDirection = Right;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && (!m_moving || m_currentDirection == Down))
    {
        m_moving = true;
        m_currentDirection = Down;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && (!m_moving || m_currentDirection == Up))
    {
        m_moving = true;
        m_currentDirection = Up;
    }
}

/**
 * @brief Load the Character from the file (snakeCase form of the name)
 * @param name Name of the character, used to find the file
 * @param characterRessourcesDir Directory where are stored the characters files and sprites
 * @return Return true if the load was successfull
 */
bool CharacterGUI::load(const std::string &name, const std::string &characterRessourcesDir)
{
    std::ifstream file(characterRessourcesDir + "/" + Tools::snakeCase(name) + ".json");
    if (file.is_open())
    {
        json json;
        file >> json;
        if (!json.is_object())
            return false;
        if (!json.contains(SPRITE_SETS) || !json[SPRITE_SETS].is_array())
            return false;
        if (!json.contains(ACTIONS) || !json[ACTIONS].is_object())
            return false;

        if (!json.contains(SPRITE_PERIOD) || !json[SPRITE_PERIOD].is_number_unsigned())
            return false;
        m_spriteChangeTics = json[SPRITE_PERIOD].get<unsigned int>();

        for (auto set : json[SPRITE_SETS])
        {
            if (!set.is_object())
                return false;
            if (!set.contains(FIRST_ID) || !set[FIRST_ID].is_number_unsigned())
                return false;
            if (!set.contains(SET_FILE) || !set[SET_FILE].is_string())
                return false;
            if (!set.contains(HEIGHT) || !set[HEIGHT].is_number_unsigned())
                return false;
            if (!set.contains(WIDTH) || !set[WIDTH].is_number_unsigned())
                return false;
            if (!set.contains(SPRITE_HEIGHT) || !set[SPRITE_HEIGHT].is_number_unsigned())
                return false;
            if (!set.contains(SPRITE_WIDTH) || !set[SPRITE_WIDTH].is_number_unsigned())
                return false;
            if (!set.contains(HORIZONTAL_SHIFT) || !set[HORIZONTAL_SHIFT].is_number_unsigned())
                return false;
            if (!set.contains(VERTICAL_SHIFT) || !set[VERTICAL_SHIFT].is_number_unsigned())
                return false;
            if (!set.contains(ORIGIN_X) || !set[ORIGIN_X].is_number_unsigned())
                return false;
            if (!set.contains(ORIGIN_Y) || !set[ORIGIN_Y].is_number_unsigned())
                return false;

            unsigned int id = 0;

            id = set[FIRST_ID].get<unsigned int>();

            std::string file = characterRessourcesDir + '/' + set[SET_FILE].get<std::string>();
            if (!std::filesystem::exists(file))
                return false;
            sf::Image image;
            image.loadFromFile(file);

            unsigned int height = set[HEIGHT].get<unsigned int>();
            unsigned int width = set[WIDTH].get<unsigned int>();

            unsigned int spriteHeight = set[SPRITE_HEIGHT].get<unsigned int>();
            unsigned int spriteWidth = set[SPRITE_WIDTH].get<unsigned int>();

            unsigned int hShift = set[HORIZONTAL_SHIFT].get<unsigned int>();
            unsigned int vShift = set[VERTICAL_SHIFT].get<unsigned int>();

            sf::Vector2f origin(set[ORIGIN_X].get<unsigned int>(), set[ORIGIN_Y].get<unsigned int>());
            // Remove backgroung if specified
            if (set.contains(BACKGROUND) && set[BACKGROUND].is_array())
            {
                if (set[BACKGROUND].size() != 3)
                    return false;
                sf::Color backgroundColor(set[BACKGROUND].at(0), set[BACKGROUND].at(1),set[BACKGROUND].at(2));
                image.createMaskFromColor(backgroundColor);
            }

            m_textures.push_back(std::make_shared<sf::Texture>());
            m_textures.back()->loadFromImage(image);

            for (unsigned int i = 0; i < height ; i++)
            {
                for (unsigned int j = 0; j < width; j++)
                {
                    m_sprites.emplace(std::pair<unsigned int, sf::Sprite>(id, sf::Sprite(*m_textures.back(),
                                    sf::IntRect(hShift + j * spriteWidth,vShift + i * spriteHeight,
                                                spriteWidth, spriteHeight))));
                    m_sprites[id].setOrigin(origin);
                    id++;
                }
            }

        }
        // Get all the actions
        for (auto a : json[ACTIONS].items())
        {
            if (!a.value().is_array())
                return false;
            std::vector<unsigned int> spriteList;
            for (auto& sprite : a.value())
            {
                if (!sprite.is_number_unsigned())
                    return false;
                spriteList.push_back(sprite.get<unsigned int>());
            }
            m_actions[a.key()] = spriteList;
        }

        for (const auto& actionName : m_requiredActions)
        {
            if (m_actions.find(actionName) == m_actions.end())
                return false;
        }

        // Initialize the first sprite
        m_currentSprite = &(m_sprites[m_actions[actions::DOWN_STOPPED].front()]);

    }
    else
        LOG(WARNING) << "Warning : Impossible to open " << characterRessourcesDir <<
                        "/" + Tools::snakeCase(name) << ".json";

    return true;
}

/**
 * @brief Method to execute when a key is released : Reset tics and cinematic index to have coherent turns
 * @param key Key released
 */
void CharacterGUI::eventKeyReleased(sf::Event::KeyEvent key)
{
    if ((key.code == sf::Keyboard::Left && m_currentDirection == Left) ||
            (key.code == sf::Keyboard::Right && m_currentDirection == Right) ||
            (key.code == sf::Keyboard::Up && m_currentDirection == Up) ||
            (key.code == sf::Keyboard::Down && m_currentDirection == Down))
    {
        m_moving = false;
        m_tics = 0;
        m_spriteCinematicIndex = 0;
    }

}

/**
 * @brief Draw the Character on the target
 * @param target Target to draw on
 * @param states Render states to use
 */
void CharacterGUI::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(*m_currentSprite, states);
}

} // namespace character::GUI
