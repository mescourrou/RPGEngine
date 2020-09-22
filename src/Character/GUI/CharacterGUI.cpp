#include "CharacterGUI.hpp"

// Stl
#include <fstream>
#ifdef BUILD_USE_FILESYSTEM
#include <filesystem>
#endif

// Project
#include <Tools.hpp>
#include <GameGUI.hpp>
#include <Character.hpp>
#include <ConfigFiles.hpp>
#include <InstrumentationTimer.hpp>

// External libs
#include <SFML/Graphics/Shape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

namespace character::gui
{

/**
 * @fn void CharacterGUI::connectSignals(game::gui::GameGUI *game, CharacterGUI *character, bool player)
 * @brief Connect signals between GameGUI and CharacterGUI
 * @param game GameGUI to connect
 * @param character CharacterGUI to connect
 * @param player If the character is the player
 */
void CharacterGUI::connectSignals(game::gui::GameGUI* game,
                                  CharacterGUI* character, bool player)
{
    PROFILE_FUNCTION();
    if (player)
    {
        game->signalArroyIsPressed.subscribeAsync(character,
                &CharacterGUI::slotArrowPressed);
        game->signalKeyReleased.subscribeAsync(character,
                                               &CharacterGUI::slotKeyReleased);
    }
}

/**
 * @fn void CharacterGUI::connectSignals(Character *character, CharacterGUI *characterGUI, bool player)
 * @brief Connect signals between frontend and backend Character
 * @param character Backend
 * @param characterGUI Frontend
 * @param player If the character is the player
 */
void CharacterGUI::connectSignals(Character*,
                                  CharacterGUI*, bool)
{
    // Nothing to do yet
}

/**
 * @brief Constructor
 */
CharacterGUI::CharacterGUI(std::weak_ptr<Character> character,
                           std::shared_ptr<config::Context> context) :
    m_character(character), m_context(context),
    m_requiredActions({actions::UP, actions::DOWN, actions::LEFT, actions::RIGHT,
                      actions::UP_STOPPED, actions::DOWN_STOPPED, actions::LEFT_STOPPED, actions::RIGHT_STOPPED})
{

}


/**
 * @brief Implementation of BaseGUIObject::prepare : change the sprite according to the current state (moving and direction)
 */
void CharacterGUI::prepare(const sf::Vector2f&)
{
    PROFILE_FUNCTION();
    auto sinceLastSprite = std::chrono::duration_cast<std::chrono::milliseconds>
                           (std::chrono::high_resolution_clock::now() - m_spriteTimer).count();
    if (sinceLastSprite >= m_spritePeriod)
    {
        unsigned int missedSprites = std::floor(sinceLastSprite / (m_spritePeriod + 1));
        auto actualiseCurrentSprite = [this,
                                       &missedSprites](const std::vector<unsigned int>& action)
        {
            m_spriteCinematicIndex += missedSprites;
            if (m_spriteCinematicIndex >= action.size())
                m_spriteCinematicIndex = 0;
            m_currentSprite = &(m_sprites[action.at(m_spriteCinematicIndex)]);
            m_spriteCinematicIndex++;
            if (m_spriteCinematicIndex >= action.size())
                m_spriteCinematicIndex = 0;
        };
        if (m_moving && m_currentDirection == Left)
            actualiseCurrentSprite(m_actions[actions::LEFT]);
        else if (m_moving && m_currentDirection == Right)
            actualiseCurrentSprite(m_actions[actions::RIGHT]);
        else if (m_moving && m_currentDirection == Down)
            actualiseCurrentSprite(m_actions[actions::DOWN]);
        else if (m_moving && m_currentDirection == Up)
            actualiseCurrentSprite(m_actions[actions::UP]);
        // If not moving, we cycle the stopped sprites
        if (!m_moving)
        {
            switch (m_currentDirection)
            {
            case Left:
                actualiseCurrentSprite(m_actions[actions::LEFT_STOPPED]);
                break;
            case Right:
                actualiseCurrentSprite(m_actions[actions::RIGHT_STOPPED]);
                break;
            case Up:
                actualiseCurrentSprite(m_actions[actions::UP_STOPPED]);
                break;
            case Down:
                actualiseCurrentSprite(m_actions[actions::DOWN_STOPPED]);
                break;
            default:
                throw BaseException("Unknown direction");
                break;
            }
        }
        m_spriteTimer = std::chrono::high_resolution_clock::now();
    }

    setOnScreenPosition(currentMap().lock()->positionOnScreenFrom(
                            m_character.lock()->position()));
}

/**
 * @brief Set the position of the current sprite on the screen
 * @param position Position of the sprite
 */
void CharacterGUI::setOnScreenPosition(const sf::Vector2f& position)
{
    PROFILE_FUNCTION();
    setPosition(position);
    m_currentSprite->setPosition(position);
}

/**
 * @brief Verify the top structure of the json file
 * @param json Json to verify
 * @return Return true if all needed fields are presents
 */
bool CharacterGUI::verifyJSONTopStructure(const json& json)
{
    PROFILE_FUNCTION();
    namespace characterFile = config::structure::characterFile;
    if (!json.is_object())
        return false;
    if (!json.contains(characterFile::SPRITE_SETS)
            || !json[characterFile::SPRITE_SETS].is_array())
        return false;
    if (!json.contains(characterFile::ACTIONS)
            || !json[characterFile::ACTIONS].is_object())
        return false;

    if (!json.contains(characterFile::SPRITE_PERIOD)
            || !json[characterFile::SPRITE_PERIOD].is_number_unsigned())
        return false;
    return true;
}

/**
 * @brief Verify the structure of the sprite sets
 * @param set Set to verify
 * @return Return true if all needed fields are presents
 */
bool CharacterGUI::verifyJSONSpriteSetsStructure(const json& set)
{
    PROFILE_FUNCTION();
    namespace characterFile = config::structure::characterFile;
    if (!set.is_object())
        return false;
    if (!set.contains(characterFile::FIRST_ID)
            || !set[characterFile::FIRST_ID].is_number_unsigned())
        return false;
    if (!set.contains(characterFile::SET_FILE)
            || !set[characterFile::SET_FILE].is_string())
        return false;
    if (!set.contains(characterFile::HEIGHT)
            || !set[characterFile::HEIGHT].is_number_unsigned())
        return false;
    if (!set.contains(characterFile::WIDTH)
            || !set[characterFile::WIDTH].is_number_unsigned())
        return false;
    if (!set.contains(characterFile::SPRITE_HEIGHT)
            || !set[characterFile::SPRITE_HEIGHT].is_number_unsigned())
        return false;
    if (!set.contains(characterFile::SPRITE_WIDTH)
            || !set[characterFile::SPRITE_WIDTH].is_number_unsigned())
        return false;
    if (!set.contains(characterFile::HORIZONTAL_SHIFT)
            || !set[characterFile::HORIZONTAL_SHIFT].is_number_unsigned())
        return false;
    if (!set.contains(characterFile::VERTICAL_SHIFT)
            || !set[characterFile::VERTICAL_SHIFT].is_number_unsigned())
        return false;
    if (!set.contains(characterFile::ORIGIN_X)
            || !set[characterFile::ORIGIN_X].is_number_unsigned())
        return false;
    if (!set.contains(characterFile::ORIGIN_Y)
            || !set[characterFile::ORIGIN_Y].is_number_unsigned())
        return false;
    return true;
}

/**
 * @brief Load the sprite character sprite sets
 * @param json Describing json
 * @param characterRessourcesDir Directory where are character assets
 * @return Return true if the loading was successful
 */
bool CharacterGUI::loadSets(const json& json,
                            const std::string& characterRessourcesDir)
{
    PROFILE_FUNCTION();
    namespace characterFile = config::structure::characterFile;
    for (auto set : json[characterFile::SPRITE_SETS])
    {
        if (!verifyJSONSpriteSetsStructure(set))
            return false;

        unsigned int id = 0;

        id = set[characterFile::FIRST_ID].get<unsigned int>();

        std::string filename = characterRessourcesDir + '/' +
                               set[characterFile::SET_FILE].get<std::string>();
        if (!std::filesystem::exists(filename))
            return false;
        sf::Image image;
        image.loadFromFile(filename);

        unsigned int height = set[characterFile::HEIGHT].get<unsigned int>();
        unsigned int width = set[characterFile::WIDTH].get<unsigned int>();

        unsigned int spriteHeight =
            set[characterFile::SPRITE_HEIGHT].get<unsigned int>();
        unsigned int spriteWidth = set[characterFile::SPRITE_WIDTH].get<unsigned int>();

        unsigned int hShift = set[characterFile::HORIZONTAL_SHIFT].get<unsigned int>();
        unsigned int vShift = set[characterFile::VERTICAL_SHIFT].get<unsigned int>();

        sf::Vector2f origin(set[characterFile::ORIGIN_X].get<unsigned int>(),
                            set[characterFile::ORIGIN_Y].get<unsigned int>());
        // Remove backgroung if specified
        if (set.contains(characterFile::BACKGROUND)
                && set[characterFile::BACKGROUND].is_array())
        {
            if (set[characterFile::BACKGROUND].size() != 3)
                return false;
            sf::Color backgroundColor(set[characterFile::BACKGROUND].at(0),
                                      set[characterFile::BACKGROUND].at(1), set[characterFile::BACKGROUND].at(2));
            image.createMaskFromColor(backgroundColor);
        }

        m_textures.push_back(std::make_shared<sf::Texture>());
        m_textures.back()->loadFromImage(image);

        for (unsigned int i = 0; i < height ; i++)
        {
            for (unsigned int j = 0; j < width; j++)
            {
                m_sprites.emplace(std::pair<unsigned int, sf::Sprite>(id,
                                  sf::Sprite(*m_textures.back(),
                                             sf::IntRect(hShift + j * spriteWidth, vShift + i * spriteHeight,
                                                     spriteWidth, spriteHeight))));
                m_sprites[id].setOrigin(origin);
                id++;
            }
        }

    }
    return true;
}

/**
 * @brief Load the differents Character actions
 * An action is a possible movement of the character
 * @param json Json describing the actions
 * @return Return true if the loading was successful
 */
bool CharacterGUI::loadActions(const json& json)
{
    PROFILE_FUNCTION();
    namespace characterFile = config::structure::characterFile;
    // Get all the actions
    for (auto a : json[characterFile::ACTIONS].items())
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

    return true;
}

/**
 * @brief Load the Character from the file (snakeCase form of the name)
 * @param characterRessourcesDir Directory where are stored the characters files and sprites
 * @return Return true if the load was successfull
 */
bool CharacterGUI::load(const std::string& characterRessourcesDir)
{
    PROFILE_FUNCTION();
    std::ifstream file(characterRessourcesDir + "/" + Tools::snakeCase(
                           m_character.lock()->name()) + ".json");
    if (file.is_open())
    {
        namespace characterFile = config::structure::characterFile;
        json json;
        file >> json;
        if (!verifyJSONTopStructure(json))
            return false;

        m_spritePeriod = json[characterFile::SPRITE_PERIOD].get<unsigned int>();

        if (!loadSets(json, characterRessourcesDir))
            return false;
        if (!loadActions(json))
            return false;

    }
    else
        LOG(WARNING) << "Warning : Impossible to open " << characterRessourcesDir <<
                     "/" + Tools::snakeCase(m_character.lock()->name()) << ".json";

    return true;
}

/**
 * @brief Slot to move the character when arrows are pressed
 * @param arrow Arrow pressed
 */
void CharacterGUI::slotArrowPressed(sf::Keyboard::Key arrow)
{
    PROFILE_FUNCTION();
    double speed = 50 * m_context->framePeriod * 0.00000001;
    if (arrow == sf::Keyboard::Left && (!m_moving || m_currentDirection == Left))
    {
        m_moving = true;
        m_currentDirection = Left;
        m_character.lock()->move({-speed, 0});
    }
    else if (arrow == sf::Keyboard::Right && (!m_moving
             || m_currentDirection == Right))
    {
        m_moving = true;
        m_currentDirection = Right;
        m_character.lock()->move({speed, 0});
    }
    else if (arrow == sf::Keyboard::Down && (!m_moving
             || m_currentDirection == Down))
    {
        m_moving = true;
        m_currentDirection = Down;
        m_character.lock()->move({0, speed});
    }
    else if (arrow == sf::Keyboard::Up && (!m_moving || m_currentDirection == Up))
    {
        m_moving = true;
        m_currentDirection = Up;
        m_character.lock()->move({0, -speed});
    }
}

/**
 * @brief Slot to manage the release of arrow keys
 * @param key Key released
 */
void CharacterGUI::slotKeyReleased(sf::Event::KeyEvent key)
{
    PROFILE_FUNCTION();
    if ((key.code == sf::Keyboard::Left && m_currentDirection == Left) ||
            (key.code == sf::Keyboard::Right && m_currentDirection == Right) ||
            (key.code == sf::Keyboard::Up && m_currentDirection == Up) ||
            (key.code == sf::Keyboard::Down && m_currentDirection == Down))
    {
        m_moving = false;
        m_spriteCinematicIndex = 0;
    }
}

/**
 * @brief Draw character basic informations for being presented to the player every time
 */
void CharacterGUI::uiRealtimeInformations()
{
    PROFILE_FUNCTION();
    ImGui::Text("%s", m_character.lock()->name().c_str());

    ImGui::ProgressBar(1, ImVec2(-1, 0), "Life");
    ImGui::ProgressBar(1, ImVec2(-1, 0), "Mana");
}

/**
 * @brief Draw the Character window
 */
void CharacterGUI::uiFullInformations()
{
    ImGui::Text("Soon");
}

/**
 * @brief Draw the inventory window
 */
void CharacterGUI::uiInventoryWindow()
{
    ImGui::Text("Soon");
}

/**
 * @brief Draw the Character on the target
 * @param target Target to draw on
 * @param states Render states to use
 */
void CharacterGUI::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    PROFILE_FUNCTION();
    target.draw(*m_currentSprite, states);
}

} // namespace character::gui
