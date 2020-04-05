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
#include <ActionHandler.hpp>
#include <WindowsManager.hpp>
#include <PerformanceTimer.hpp>
#include <InstrumentationTimer.hpp>

#include <CharacterGUI.hpp>

// External lib
#include <glog/logging.h>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <imgui-SFML.h>
#include <imgui.h>

namespace game
{

namespace gui
{

/**
 * @brief Construct the GameGUI
 * @param context Context to use
 * @param game Game attached
 */
GameGUI::GameGUI(std::shared_ptr<config::Context> context, Game* game):
    m_context(context), m_game(game)
{
    PROFILE_FUNCTION();
    VLOG(verbosityLevel::OBJECT_CREATION) << "Creating " << className() << " => " <<
                                          this;

    loadFromConfig();
    m_context->config()->signalConfigUpdated.subscribeAsync(this,
            &GameGUI::loadFromConfig);
    ImGui::SFML::Init(*m_window);

}

GameGUI::~GameGUI()
{
    PROFILE_FUNCTION();
    m_window->close();
    ImGui::SFML::Shutdown();
}

/**
 * @brief Initialize the GUI part of Game
 * @param db Database to use
 * @return Return true if the initialization went well
 */
bool GameGUI::initialize(std::shared_ptr<databaseTools::Database> db)
{
    PROFILE_FUNCTION();
    VLOG(verbosityLevel::FUNCTION_CALL) << "Initialize";
    m_drawingTimer = std::chrono::high_resolution_clock::now();

    m_mapGUI = std::make_shared<map::gui::MapGUI>(m_game->m_currentMap);

    m_mapGUI->load(m_context->kMapPath());

    m_mapGUI->setCenterOfView({m_game->m_playerCharacter->position().x(),
                               m_game->m_playerCharacter->position().y()});

    m_game->m_playerCharacter->signalPositionChanged.subscribeSync([this](
                const map::Position & pos)
    {
        m_mapGUI->setCenterOfView({pos.x(), pos.y()});
    });

    m_player = addGUIObject<character::gui::CharacterGUI>(m_game->m_playerCharacter,
               m_context);

    m_player.lock()->load(m_context->kCharacterPath());
    character::gui::CharacterGUI::connectSignals(this, m_player.lock().get(), true);
    character::gui::CharacterGUI::connectSignals(m_game->m_playerCharacter.get(),
            m_player.lock().get(), true);

    signalPause.subscribeAsync([this](bool pause)
    {
        m_ui.onPause = pause;
    });

    events::ActionHandler::addAction(CHARACTER_WINDOW_ACTION, [this]()
    {
        m_characterWindow->setActive(!m_characterWindow->active());
    });

    events::ActionHandler::addAction(INVENTORY_WINDOW_ACTION, [this]()
    {
        m_inventoryWindow->setActive(!m_inventoryWindow->active());
    });

    m_characterWindow = std::make_unique<CharacterWindow>();
    m_characterWindow->setTitle(m_game->m_playerCharacter->name());
    m_characterWindow->setActive(false);
    m_windowsManager.addWindow(m_characterWindow.get());

    m_inventoryWindow = std::make_unique<InventoryWindow>();
    m_inventoryWindow->setActive(false);
    m_windowsManager.addWindow(m_inventoryWindow.get());
    return true;
}

/**
 * @brief Manage the SFML events
 */
void GameGUI::eventManager()
{
    PROFILE_FUNCTION();
    static sf::Clock deltaClock;
    ImGui::SFML::Update(*m_window, deltaClock.restart());
    // Process events
    while (m_window->pollEvent(m_event))
    {
        ImGui::SFML::ProcessEvent(m_event);
        // Close window: exit
        if (m_event.type == sf::Event::Closed)
        {
            m_signalOnClose.trigger();
        }
        if (m_event.type == sf::Event::KeyPressed)
            managePressingKeyEvent(m_event.key);

        if (m_event.type == sf::Event::KeyReleased)
            manageReleasingKeyEven(m_event.key);
    }
    checkKeyPressed();

    makeUI();

}

/**
 * @brief Draw the window and the widgets
 */
void GameGUI::draw()
{
    PROFILE_FUNCTION();
    m_windowsManager.prepareWindows();
    m_mapGUI->prepare(m_window->getView().getSize());
    std::sort(m_guiObjects.begin(),
              m_guiObjects.end(), [](std::shared_ptr<BaseGUIObject> obj1,
                                     std::shared_ptr<BaseGUIObject> obj2)
    {
        return obj1->getPosition().y < obj2->getPosition().y;
    });
    for (auto& obj : m_guiObjects)
    {
        if (obj)
        {
            obj->setCurrentMap(m_mapGUI);
            obj->prepare(m_window->getView().getSize());
        }
    }
    std::this_thread::sleep_until(m_drawingTimer + m_drawingPeriod);
    m_window->clear();
    m_window->draw(*m_mapGUI);

    for (auto& obj : m_guiObjects)
    {
        if (obj)
            m_window->draw(*obj);
    }

    ImGui::SFML::Render(*m_window);
    m_window->display();
    m_drawingTimer = std::chrono::high_resolution_clock::now();
}

/**
 * @brief Load the elements from the config file. Called everytime the config is saved
 */
void GameGUI::loadFromConfig()
{
    PROFILE_FUNCTION();
    if (m_window)
        m_window->close();
    namespace structure = config::structure::globalFile;
    bool fullscreen = false;
    if (m_context->config()->getValue(structure::preferences::SECTION,
                                      structure::preferences::FULLSCREEN) == "true")
        fullscreen = true;

    std::string strResolution = m_context->config()->getValue(
                                    structure::preferences::SECTION, structure::preferences::RESOLUTION);

    if (strResolution.empty())
        strResolution = GAME_DEFAULT_RESOLUTION;

    auto xIndex = strResolution.find('x');
    int xResolution = std::atoi(strResolution.substr(0, xIndex).c_str());
    int yResolution = std::atoi(strResolution.substr(xIndex + 1, -1).c_str());

    VLOG(verbosityLevel::VERIFICATION_LOG) << "Resolution = " << xResolution <<
                                           " x " << yResolution;

    if (!fullscreen)
        m_window = std::make_shared<sf::RenderWindow>(sf::VideoMode(xResolution,
                   yResolution), "RPGEngine");
    else
        m_window = std::make_shared<sf::RenderWindow>(sf::VideoMode(xResolution,
                   yResolution), "RPGEngine", sf::Style::Fullscreen);

}

/**
 * @brief Create the User Interface using Dear ImGui
 */
void GameGUI::makeUI()
{
    PROFILE_FUNCTION();
    if (m_ui.uiActivated)
    {
        if (ImGui::Begin(UI::BOTTON_AREA, nullptr, UI::FIXED))
        {
            ImGui::GetStyle().WindowRounding = 0.0F;
            ImGui::SetWindowSize(ImVec2(m_window->getSize().x, 0));

            ImGui::Columns(2);
            m_player.lock()->uiRealtimeInformations();
            if (ImGui::Button(UI::INVENTORY_BUTTON))
            {
                events::ActionHandler::execute(INVENTORY_WINDOW_ACTION);
            }
            ImGui::SameLine();
            if (ImGui::Button(UI::CHARACTER_BUTTON))
            {
                events::ActionHandler::execute(CHARACTER_WINDOW_ACTION);
            }

            ImGui::NextColumn();
            // Complete with abilities

            ImGui::SetWindowPos(ImVec2(0,
                                       m_window->getSize().y - ImGui::GetWindowHeight()));
        }
        ImGui::End(); // Bottom Area
    }
    if (m_ui.onPause)
        ImGui::OpenPopup(UI::PAUSE_POPUP);
    if (m_ui.onPause
            && ImGui::BeginPopupModal(UI::PAUSE_POPUP, nullptr,
                                      ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
    {
        uiPauseMenu();
        ImGui::EndPopup();
    }

}

/**
 * @brief Generate the UI pause menu
 */
void GameGUI::uiPauseMenu()
{
    PROFILE_FUNCTION();
    // Main pause menu
    if (ImGui::Button("Return to the game"))
        signalPause.trigger(false);

    if (ImGui::Button("Settings"))
    {
        ImGui::OpenPopup(UI::SETTINGS_POPUP);
        uiLoadSettingsPopup();
    }

    if (ImGui::Button("Informations"))
        ImGui::OpenPopup(UI::INFOS_POPUP);

    if (ImGui::Button("Exit"))
    {
        m_signalOnClose.trigger();
        exit(EXIT_SUCCESS);
    }

    // Information popup
    if (ImGui::BeginPopupModal(UI::INFOS_POPUP))
        uiInformationPopup();

    // Settings popup
    if (ImGui::BeginPopupModal(UI::SETTINGS_POPUP, nullptr,
                               ImGuiWindowFlags_AlwaysAutoResize))
        uiSettingsPopup();

}

/**
 * @brief Load the settings locally
 */
void GameGUI::uiLoadSettingsPopup()
{
    PROFILE_FUNCTION();
    namespace preferences = config::structure::globalFile::preferences;
    m_ui.settings.fullscreen = false;
    std::string tmp = m_context->config()->getValue(preferences::SECTION,
                      preferences::FULLSCREEN);
    if (tmp == "true")
        m_ui.settings.fullscreen = true;

    m_ui.settings.resolution = m_context->config()->getValue(preferences::SECTION,
                               preferences::RESOLUTION);
    if (m_ui.settings.resolution.empty())
        m_ui.settings.resolution = GAME_DEFAULT_RESOLUTION;

    m_ui.settings.availableResolutions = {"1920x1080", "900x600"};

    m_ui.settings.resolutionItemSelected = static_cast<int>(
            std::distance(m_ui.settings.availableResolutions.begin(),
                          std::find(m_ui.settings.availableResolutions.begin(),
                                    m_ui.settings.availableResolutions.end(),
                                    m_ui.settings.resolution.c_str())));

    if (m_ui.settings.resolutionItemSelected == static_cast<int>
            (m_ui.settings.availableResolutions.size()))
        m_ui.settings.availableResolutions.push_back(m_ui.settings.resolution.c_str());
}

void GameGUI::uiInformationPopup()
{
    PROFILE_FUNCTION();
    if (ImGui::CollapsingHeader("Game", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::Text("Name : %s", m_game->name().c_str());
        ImGui::Text("Game directory : %s", m_context->gameLocation().c_str());
    }
    if (ImGui::CollapsingHeader("Engine", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::Text("Version : %d.%d.%d", VERSION_MAJOR, VERSION_MINOR, VERSION_BUILD);
        ImGui::Text("Build date : %s %s", __DATE__, __TIME__);
#ifdef GIT_BRANCH
        ImGui::Text("Branch : %s", GIT_BRANCH);
#endif
#ifdef GIT_COMMIT_HASH
        ImGui::Text("Commit hash : %s", GIT_COMMIT_HASH);
#endif
        ImGui::Text("Engine location : %s", m_context->runtimeDirectory().c_str());
    }
    ImGui::EndPopup();
}

void GameGUI::uiSettingsPopup()
{
    PROFILE_FUNCTION();
    if (ImGui::BeginTabBar(UI::SETTINGS_TABBAR_NAME))
    {
        namespace preferences = config::structure::globalFile::preferences;
        if (ImGui::BeginTabItem(preferences::SECTION))
        {
            ImGui::Checkbox("Fullscreen", &m_ui.settings.fullscreen);

            ImGui::ListBox("Resolution", &m_ui.settings.resolutionItemSelected,
                           static_cast<const char**>(&m_ui.settings.availableResolutions[0]),
                           m_ui.settings.availableResolutions.size());

            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Key binding"))
        {
            auto actionList = events::ActionHandler::actionList();
            for (const std::string& actionName : actionList)
            {
                ImGui::PushID(actionName.c_str());
                ImGui::Text("%s", actionName.c_str());
                ImGui::SameLine();
                ImGui::Text(" -- ");
                ImGui::SameLine();
                if (ImGui::Button(events::ActionHandler::getKeyBinding(
                                      actionName).toString().c_str()))
                {
                    m_actionWaitingForKeybinding = actionName;
                }
                ImGui::PopID();
            }
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();

    }
    if (ImGui::Button("Save"))
    {
        namespace preferences = config::structure::globalFile::preferences;
        auto config = m_context->config();
        config->setValue(preferences::SECTION, preferences::FULLSCREEN,
                         (m_ui.settings.fullscreen ? "true" : "false"));

        config->setValue(preferences::SECTION, preferences::RESOLUTION,
                         (m_ui.settings.availableResolutions.at(m_ui.settings.resolutionItemSelected)));
        config->saveToFile();
    }


    ImGui::EndPopup();
}

void GameGUI::managePressingKeyEvent(const sf::Event::KeyEvent& key)
{
    PROFILE_FUNCTION();
    if (m_actionWaitingForKeybinding.empty())
    {
        signalKeyPressed.trigger(key);
        events::ActionHandler::processSFMLEvent(key);
    }
}

void GameGUI::manageReleasingKeyEven(const sf::Event::KeyEvent& key)
{
    PROFILE_FUNCTION();
    if (m_actionWaitingForKeybinding.empty())
    {
        switch (key.code)
        {
        case sf::Keyboard::Escape:
            signalPause.trigger(!m_ui.onPause);
            break;
        case sf::Keyboard::U:
            m_ui.uiActivated = !m_ui.uiActivated;
            break;
        default:
            break;
        }
        signalKeyReleased.trigger(key);
    }
    else
    {
        auto keyBinding = events::KeyBinding::fromSFML(key);
        if (!keyBinding.isKey(events::KeyBinding::NOT_BINDED, events::KeyBinding::NONE))
        {
            events::ActionHandler::setKeyBinding(m_actionWaitingForKeybinding, keyBinding);
            m_actionWaitingForKeybinding = "";
        }
    }
}

void GameGUI::checkKeyPressed()
{
    PROFILE_FUNCTION();
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        signalArroyIsPressed.trigger(sf::Keyboard::Left);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        signalArroyIsPressed.trigger(sf::Keyboard::Right);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        signalArroyIsPressed.trigger(sf::Keyboard::Down);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        signalArroyIsPressed.trigger(sf::Keyboard::Up);
}

} // namespace gui

} // namespace map
