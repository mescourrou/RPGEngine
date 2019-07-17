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

#include <imgui-SFML.h>
#include <imgui.h>

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

    ImGui::SFML::Init(*m_window);
}

/**
 * @brief Initialize the GUI part of Game
 * @param db Database to use
 * @return Return true if the initialization went well
 */
bool GameGUI::initialize(std::shared_ptr<database::Database> db)
{
    VLOG(verbosityLevel::FUNCTION_CALL) << "Initialize";

    m_mapGUI = std::make_shared<map::GUI::MapGUI>(m_game->m_currentMap);

    m_mapGUI->load(m_context->kMapPath());

    m_mapGUI->setCenterOfView({m_game->m_playerCharacter->position().x(),
                               m_game->m_playerCharacter->position().y()});

    m_game->m_playerCharacter->signalPositionChanged.subscribeSync([this](map::Position pos){
        m_mapGUI->setCenterOfView({pos.x(), pos.y()});
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
        ImGui::SFML::ProcessEvent(event);
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
    static sf::Clock deltaClock;
    ImGui::SFML::Update(*m_window, deltaClock.restart());

            ImGui::Begin("Hello, world!");
            ImGui::Button("Look at this pretty button");
            ImGui::End();

}

/**
 * @brief Draw the window and the widgets
 */
void GameGUI::draw()
{
    m_mapGUI->prepare(m_window->getSize());
    for (auto& obj : m_guiObjects)
    {
        if (obj)
        {
            obj->setCurrentMap(m_mapGUI);
            obj->prepare(m_window->getSize());
        }
    }
    m_window->clear();
    m_window->draw(*m_mapGUI);

    for (auto& obj : m_guiObjects)
    {
        if (obj)
            m_window->draw(*obj);
    }

    ImGui::SFML::Render(*m_window);
    m_window->display();
}

} // namespace GUI

} // namespace map
