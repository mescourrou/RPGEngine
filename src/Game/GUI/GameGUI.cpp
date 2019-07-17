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

    m_player = addGUIObject<character::GUI::CharacterGUI>(m_game->m_playerCharacter);

    m_player.lock()->load(m_context->kCharacterPath());
    character::GUI::CharacterGUI::connectSignals(this, m_player.lock().get(), true);
    character::GUI::CharacterGUI::connectSignals(m_game->m_playerCharacter.get(), m_player.lock().get(), true);

    m_signalOnClose.subscribeSync([this](){
        m_window->close();
        ImGui::SFML::Shutdown();}
    );

    signalPause.subscribeSync([this](bool pause){
        m_ui.onPause = pause;
    });

    return true;
}

/**
 * @brief Manage the SFML events
 */
void GameGUI::eventManager()
{
    static sf::Clock deltaClock;
    ImGui::SFML::Update(*m_window, deltaClock.restart());
    // Process events
    sf::Event event;
    while (m_window->pollEvent(event))
    {
        ImGui::SFML::ProcessEvent(event);
        // Close window: exit
        if (event.type == sf::Event::Closed)
        {
            m_signalOnClose.trigger();
            exit(EXIT_SUCCESS);
        }
        if (event.type == sf::Event::KeyPressed)
        {
            signalKeyPressed.trigger(event.key);
        }
        if (event.type == sf::Event::KeyReleased)
        {
            switch (event.key.code) {
            case sf::Keyboard::Escape:
                signalPause.trigger(!m_ui.onPause);
                break;
            case sf::Keyboard::U:
                m_ui.uiActivated = !m_ui.uiActivated;
                break;
            }
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

    makeUI();

}

/**
 * @brief Draw the window and the widgets
 */
void GameGUI::draw()
{
    m_mapGUI->prepare(m_window->getSize());
    std::sort(m_guiObjects.begin(), m_guiObjects.end(), [](std::shared_ptr<BaseGUIObject> obj1, std::shared_ptr<BaseGUIObject> obj2){
       return obj1->getPosition().y < obj2->getPosition().y;
    });

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

/**
 * @brief Create the User Interface using Dear ImGui
 */
void GameGUI::makeUI()
{
    if (m_ui.uiActivated)
    {
        if (ImGui::Begin(UI::BOTTON_AREA, nullptr, UI::FIXED))
        {
            ImGui::GetStyle().WindowRounding = 0.0f;
            ImGui::SetWindowSize(ImVec2(m_window->getSize().x, 0));

            ImGui::Columns(2);
            m_player.lock()->uiRealtimeInformations();
            if (ImGui::Button(UI::INVENTORY_BUTTON))
            {
                m_ui.inventoryOpen = !m_ui.inventoryOpen;
            }
            ImGui::SameLine();
            if (ImGui::Button(UI::CHARACTER_BUTTON))
            {
                m_ui.characterOpen = !m_ui.characterOpen;
            }

            ImGui::NextColumn();
            // Complete with abilities

            ImGui::SetWindowPos(ImVec2(0,m_window->getSize().y - ImGui::GetWindowHeight()));
        }
        ImGui::End(); // Bottom Area

        if (m_ui.inventoryOpen)
        {
            if (ImGui::Begin(UI::INVENTORY_BUTTON, nullptr, ImGuiWindowFlags_NoSavedSettings))
            {
                m_player.lock()->uiInventoryWindow();
            }
            else // Window collapsed
            {
                ImGui::SetWindowCollapsed(false);
                m_ui.inventoryOpen = false;
            }
            ImGui::End();
        }
        if (m_ui.characterOpen)
        {
            if (ImGui::Begin(m_game->m_playerCharacter->name().c_str(), nullptr, ImGuiWindowFlags_NoSavedSettings))
            {
                m_player.lock()->uiFullInformations();
            }
            else // Window collapsed
            {
                ImGui::SetWindowCollapsed(false);
                m_ui.characterOpen = false;
            }
            ImGui::End();
        }
    }
    if (m_ui.onPause)
        ImGui::OpenPopup(UI::PAUSE_POPUP);
    if (m_ui.onPause && ImGui::BeginPopupModal(UI::PAUSE_POPUP, nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
    {
        if (ImGui::Button("Return to the game"))
            signalPause.trigger(false);
        if (ImGui::Button("Informations"))
        {
            ImGui::OpenPopup(UI::INFOS_POPUP);
        }
        if (ImGui::Button("Exit"))
        {
            m_signalOnClose.trigger();
            exit(EXIT_SUCCESS);
        }
        if (ImGui::BeginPopupModal(UI::INFOS_POPUP))
        {
            if (ImGui::CollapsingHeader("Game", ImGuiTreeNodeFlags_DefaultOpen))
            {
                ImGui::Text("Name : %s", m_game->name().c_str());
                ImGui::Text("Game directory : %s", m_context->gameLocation().c_str());
            }
            if (ImGui::CollapsingHeader("Engine", ImGuiTreeNodeFlags_DefaultOpen))
            {
                ImGui::Text("Version : %d.%d.%d", VERSION_MAJOR, VERSION_MINOR, VERSION_BUILD);
                ImGui::Text("Build date : %s %s", __DATE__, __TIME__);
                ImGui::Text("Engine location : %s", m_context->runtimeDirectory().c_str());
            }
            ImGui::EndPopup();
        }

        ImGui::EndPopup();
    }

}

} // namespace GUI

} // namespace map
