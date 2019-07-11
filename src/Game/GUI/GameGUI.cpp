#include "GameGUI.hpp"

// Project
#include <Model.hpp>
#include <Database.hpp>
#include <VerbosityLevels.hpp>
#include <Query.hpp>
#include <Config.hpp>
#include <ConfigFiles.hpp>
#include <general_config.hpp>
#include <Game.hpp>
#include <Character.hpp>

#include <CharacterGUI.hpp>

// External lib
#include <glog/logging.h>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

namespace game {

namespace GUI {

/**
 * @brief Construct the GameGUI
 * @param context Context to use
 * @param game Game attached
 */
GameGUI::GameGUI(std::shared_ptr<config::Context> context, Game* game):
    m_context(context), m_game(game)
{
    VLOG(verbosityLevel::OBJECT_CREATION) << "Creating " << className() << " => " << this;
    namespace structure = config::structure::globalFile;
    bool fullscreen = false;
    if (m_context->config()->getValue(structure::preferences::SECTION, structure::preferences::FULLSCREEN) == "true")
        fullscreen = true;

    std::string strResolution = m_context->config()->getValue(structure::preferences::SECTION, structure::preferences::RESOLUTION);

    if (strResolution.empty())
        strResolution = GAME_DEFAULT_RESOLUTION;

    auto xIndex = strResolution.find('x');
    int xResolution = std::atoi(strResolution.substr(0,xIndex).c_str());
    int yResolution = std::atoi(strResolution.substr(xIndex+1,-1).c_str());

    VLOG(verbosityLevel::VERIFICATION_LOG) << "Resolution = " << xResolution << " x " << yResolution;

    if (!fullscreen)
        m_window = std::make_shared<sf::RenderWindow>(sf::VideoMode(xResolution, yResolution), "RPGEngine");
    else
        m_window = std::make_shared<sf::RenderWindow>(sf::VideoMode(xResolution, yResolution), "RPGEngine", sf::Style::Fullscreen);
}

/**
 * @brief Initialize the GUI part of Game
 * @param db Database to use
 * @return Return true if the initialization went well
 */
bool GameGUI::initialize(std::shared_ptr<database::Database> db)
{
    VLOG(verbosityLevel::FUNCTION_CALL) << "Initialize";

    m_game->m_currentMap.lock()->setCenterOfView({m_game->m_playerCharacter->position().x(),
                                                  m_game->m_playerCharacter->position().y()});

    m_game->m_playerCharacter->signalPositionChanged.subscribeSync([this](map::Position pos){
        m_game->m_currentMap.lock()->setCenterOfView({pos.x(), pos.y()});
    });

    auto player = addGUIObject<character::GUI::CharacterGUI>(m_game->m_playerCharacter);

    player.lock()->load(m_context->kCharacterPath());
    character::GUI::CharacterGUI::connectSignals(this, player.lock().get(), true);
    character::GUI::CharacterGUI::connectSignals(m_game->m_playerCharacter.get(), player.lock().get(), true);
    return true;
}

/**
 * @brief Manage the SFML events
 */
void GameGUI::eventManager()
{
    static map::Vector<2> position;
    map::Vector<2> moveVector;
    // Process events
    sf::Event event;
    while (m_window->pollEvent(event))
    {
        // Close window: exit
        if (event.type == sf::Event::Closed)
        {
            m_window->close();
            m_signalOnClose.trigger();
        }
        if (event.type == sf::Event::KeyPressed)
        {
            signalKeyPressed.trigger(event.key);
        }
        if (event.type == sf::Event::KeyReleased)
        {
            signalKeyReleased.trigger(event.key);
        }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        signalArroyIsPressed.trigger(sf::Keyboard::Left);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        signalArroyIsPressed.trigger(sf::Keyboard::Right);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        signalArroyIsPressed.trigger(sf::Keyboard::Down);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        signalArroyIsPressed.trigger(sf::Keyboard::Up);

}

/**
 * @brief Draw the window and the widgets
 */
void GameGUI::draw()
{
    m_game->m_currentMap.lock()->setTarget(m_window);
    m_game->m_currentMap.lock()->prepare(m_window->getSize());
    for (auto& obj : m_guiObjects)
    {
        if (obj)
        {
            obj->setCurrentMap(m_game->m_currentMap);
            obj->prepare(m_window->getSize());
        }
    }
    m_window->clear();
    m_window->draw(*m_game->m_currentMap.lock());


    for (auto& obj : m_guiObjects)
    {
        if (obj)
            m_window->draw(*obj);
    }
    m_window->display();
}

} // namespace GUI

} // namespace map
