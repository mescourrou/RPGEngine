#include "MakerGUI.hpp"
#include <VerbosityLevels.hpp>
#include <Context.hpp>
#include <WorkerThread.hpp>
#include <Maker.hpp>
#include <Logger.hpp>
#include <Popups.hpp>
#include <ActionHandler.hpp>

#include <MapGUI.hpp>

#include <SFML/Graphics/RenderWindow.hpp>
#include <imgui-SFML.h>
#include <imgui.h>

#include <glog/logging.h>

namespace maker::gui
{

/**
 * @brief Construct a MakerGUI
 * @param context Context to use
 * @param maker Pointer on the maker backend
 */
MakerGUI::MakerGUI(std::shared_ptr<config::Context> context, Maker* maker) :
    m_context(context), m_maker(maker)
{

}

/**
 * @brief Close the GUI parts
 */
MakerGUI::~MakerGUI()
{
    m_window.close();
    ImGui::SFML::Shutdown();
}

/**
 * @brief Initialize the maker
 * @return Return true if all went well
 */
bool MakerGUI::initialize()
{
    VLOG(verbosityLevel::FUNCTION_CALL) << "Initialize";

    m_maker->subscribeASyncToSignalMapUpdated([this](std::weak_ptr<map::Map> mapPtr)
    {
        m_mapGUI.reset();
        m_mapGUI = std::make_shared<map::gui::MapGUI>(mapPtr);
        m_mapGUI->load(m_context->kMapPath());
    });

    m_window.create(sf::VideoMode(900, 600), "RPGEngine",
                    sf::Style::Resize | sf::Style::Close);
    ImGui::SFML::Init(m_window);

    m_characterWindow = std::make_unique<CharacterWindow>(m_maker);
    m_characterWindow->setActive(false);
    m_windowManager.addWindow(m_characterWindow.get());

    m_consoleWindow = std::make_unique<ConsoleWindow>();
    m_consoleWindow->setActive(true);
    m_windowManager.addWindow(m_consoleWindow.get());

    m_moneyWindow = std::make_unique<MoneyWindow>(m_maker);
    m_moneyWindow->setActive(false);
    m_windowManager.addWindow(m_moneyWindow.get());

    m_mapWindow = std::make_unique<MapWindow>(m_maker);
    m_mapWindow->setActive(false);
    m_windowManager.addWindow(m_mapWindow.get());

    m_dialogueWindow = std::make_unique<DialogueWindow>(m_maker);
    m_dialogueWindow->setActive(false);
    m_windowManager.addWindow(m_dialogueWindow.get());

    m_maker->getStateMachine().addExitStateAction(Maker::PROJECT_LOADING, []()
    {
        // future work
    });
    m_maker->getStateMachine().addEntryStateAction(Maker::WORKBENCH, [this]()
    {
        m_moneyWindow->setActive(true);
        m_mapWindow->setActive(true);
        m_dialogueWindow->setActive(true);
    });
    m_maker->getStateMachine().addExitStateAction(Maker::WORKBENCH, [this]()
    {
        m_moneyWindow->setActive(false);
        m_mapWindow->setActive(false);
        m_characterWindow->setActive(false);
        m_dialogueWindow->setActive(false);
    });

    events::ActionHandler::addAction("Open", [this]()
    {
        if (!m_ui.openGame.window)
        {
            m_ui.openGame.window = true;
        }
    }, events::KeyBinding(events::KeyBinding::O, events::KeyBinding::CTRL));

    events::ActionHandler::addAction("New", [this]()
    {
        m_ui.newGame.state = UI::NewGame::DIRECTORY;
    }, events::KeyBinding(events::KeyBinding::N, events::KeyBinding::CTRL));

    events::ActionHandler::addAction("Quit", [this]()
    {
        getSignalClose().trigger();
    }, events::KeyBinding(events::KeyBinding::Q, events::KeyBinding::CTRL));

    return true;
}

/**
 * @brief Maker event management
 */
void MakerGUI::eventManager()
{
    static sf::Clock deltaClock;
    ImGui::SFML::Update(m_window, deltaClock.restart());
    // Process events
    sf::Event event;
    while (m_window.pollEvent(event))
    {
        ImGui::SFML::ProcessEvent(event);
        // Close window: exit
        switch (event.type)
        {
        case sf::Event::Closed:
            events::ActionHandler::execute("Quit");
            break;
        case sf::Event::Resized:
            if (m_mapGUI)
                m_mapGUI->forcePrepare(m_window.getView().getSize());
            break;
        case sf::Event::KeyPressed:
            processKeyPressedEvent(event);
            break;
        case sf::Event::KeyReleased:
            processKeyReleasedEvent(event);
            break;
        default:
            break;
        }
    }

    makeUI();
}

/**
 * @brief Process the event resulting a key pressed
 * @param event Event to process
 */
void MakerGUI::processKeyPressedEvent(const sf::Event& event) const
{
    if (event.type != sf::Event::KeyPressed)
        return;

    events::ActionHandler::processSFMLEvent(event.key);
    switch (event.key.code)
    {
    case sf::Keyboard::Left:
        if (m_mapGUI) m_mapGUI->moveCenterOfView(-5, 0);
        break;
    case sf::Keyboard::Right:
        if (m_mapGUI) m_mapGUI->moveCenterOfView(5, 0);
        break;
    case sf::Keyboard::Up:
        if (m_mapGUI) m_mapGUI->moveCenterOfView(0, -5);
        break;
    case sf::Keyboard::Down:
        if (m_mapGUI) m_mapGUI->moveCenterOfView(0, 5);
        break;
    default:
        break;
    }
}

/**
 * @brief Process the event resulting a key released
 * @param event Event to process
 */
void MakerGUI::processKeyReleasedEvent(const sf::Event& event) const
{
    if (event.type != sf::Event::KeyReleased)
        return;

    switch (event.key.code)
    {
    case sf::Keyboard::Escape:
        break;
    default:
        break;
    }
}

/**
 * @brief Draw the maker on the window
 */
void MakerGUI::draw()
{
    if (m_mapGUI)
        m_mapGUI->prepare(m_window.getView().getSize());
    m_window.clear(sf::Color::White);
    if (m_mapGUI)
    {
        m_window.draw(*m_mapGUI);
    }
    ImGui::Popups::Draw();
    ImGui::SFML::Render(m_window);
    m_window.display();
}

/**
 * @brief Reset UI
 */
void MakerGUI::resetUI()
{
    m_ui = UI();
}

/**
 * @brief Prepare the UI elements
 */
void MakerGUI::makeUI()
{
    makeMainMenuBarUI();
    makeNewGameUI();
    makeOpenGameUI();

    m_windowManager.prepareWindows();
}

void MakerGUI::makeMainMenuBarUI()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New",
                                events::ActionHandler::getKeyBinding("New").toString().c_str()))
            {
                events::ActionHandler::execute("New");
            }
            if (ImGui::MenuItem("Open",
                                events::ActionHandler::getKeyBinding("Open").toString().c_str()))
            {
                events::ActionHandler::execute("Open");
            }
            if (ImGui::MenuItem("Save", "Ctrl+S"))
            {

            }
            if (ImGui::MenuItem("Save As", "Ctrl+Shift+S"))
            {

            }
            ImGui::Separator();
            if (ImGui::MenuItem("Quit",
                                events::ActionHandler::getKeyBinding("Quit").toString().c_str()))
            {
                events::ActionHandler::execute("Quit");
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Display"))
        {
            for (auto* w : m_windowManager.windowsList())
            {
                ImGui::Checkbox(w->title().c_str(), &w->active());
            }

            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

void MakerGUI::makeNewGameUI()
{
    if (m_ui.newGame.state == UI::NewGame::DIRECTORY)
    {
        if (!m_fileBrowser)
        {
            m_fileBrowser = std::make_unique<ImGui::FileBrowser>
                            (ImGuiFileBrowserFlags_CreateNewDir |
                             ImGuiFileBrowserFlags_SelectDirectory |
                             ImGuiFileBrowserFlags_CloseOnEsc);
            m_fileBrowser->SetTitle("New game : select the directory");
            m_fileBrowser->Open();
        }
        m_fileBrowser->Display();

        if (m_fileBrowser->HasSelected())
        {
            m_ui.newGame.directory = m_fileBrowser->GetSelected().string();
            m_ui.newGame.directory.erase(m_ui.newGame.directory.end() - 1);
            m_fileBrowser->ClearSelected();
            m_fileBrowser->Close();
            ImGui::OpenPopup("New game");

        }
        if (!m_fileBrowser->IsOpened())
        {
            m_ui.newGame.state = UI::NewGame::INFORMATIONS;
            delete m_fileBrowser.release();
        }
    }
    if (ImGui::BeginPopupModal("New game", nullptr,
                               ImGuiWindowFlags_AlwaysAutoResize))
    {
        bool valid = true;
        ImGui::InputText("Name", (char*)&m_ui.newGame.gameName, 100);
        if (strlen(m_ui.newGame.gameName) == 0)
            valid = false;

        if (valid)
        {
            if (ImGui::Button("Validate"))
            {
                m_ui.newGame.state = UI::NewGame::NONE;
                events::WorkerThread::newWork(m_maker, &Maker::doNewGame,
                                              std::string(m_ui.newGame.gameName), m_ui.newGame.directory);
                ImGui::CloseCurrentPopup();
                resetUI();
            }
            ImGui::SameLine();
        }
        if (ImGui::Button("Cancel"))
        {
            m_ui.newGame.state = UI::NewGame::NONE;
            ImGui::CloseCurrentPopup();
        }
        if (ImGui::IsRootWindowOrAnyChildFocused() && !ImGui::IsAnyItemActive()
                && !ImGui::IsMouseClicked(0))
            ImGui::SetKeyboardFocusHere(0);
        ImGui::EndPopup();
    }
}

void MakerGUI::makeOpenGameUI()
{
    if (m_ui.openGame.window)
    {
        if (ImGui::Begin("Open Game", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
        {
            if (m_ui.openGame.gameList.size() == 0)
            {

                m_ui.openGame.gameList = m_maker->gameList();
                m_ui.openGame.selectedItem = 0;
            }
            if (m_ui.openGame.gameList.size() > 0)
            {
                ImGui::ListBox("Select the game", &m_ui.openGame.selectedItem,
                               m_ui.openGame.gameList.data(),
                               m_ui.openGame.gameList.size());

                if (ImGui::Button("OK"))
                {
                    events::WorkerThread::newWork(m_maker, &Maker::doOpenGame,
                                                  m_ui.openGame.gameList.getStr(m_ui.openGame.selectedItem));
                    m_ui.openGame.window = false;
                }
            }
            else
            {
                ImGui::Text("No game for now, create a new one !");
                if (ImGui::Button("New game"))
                {
                    m_ui.newGame.state = UI::NewGame::DIRECTORY;
                    m_ui.openGame.window = false;
                }
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel"))
            {
                m_ui.openGame.window = false;
            }
        }
        ImGui::End();
    }
}

} // namespace maker::gui
