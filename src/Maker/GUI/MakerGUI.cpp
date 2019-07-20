#include "MakerGUI.hpp"
#include <VerbosityLevels.hpp>
#include <Context.hpp>
#include <WorkerThread.hpp>
#include <Maker.hpp>
#include <Logger.hpp>

#include <SFML/Graphics/RenderWindow.hpp>
#include <imgui-SFML.h>
#include <imgui.h>

#include <glog/logging.h>

namespace maker::GUI {

MakerGUI::MakerGUI(std::shared_ptr<config::Context> context, Maker *maker) :
    m_context(context), m_maker(maker)
{
    m_maker->signalCharacterListUpdated.subscribeAsync([this](std::vector<std::string> list){
        m_ui.character.list = list;
    });
}

bool MakerGUI::initialize()
{
    VLOG(verbosityLevel::FUNCTION_CALL) << "Initialize";

    signalClose.subscribeSync([this](){
        m_window->close();
        ImGui::SFML::Shutdown();}
    );

    m_window = std::make_shared<sf::RenderWindow>(sf::VideoMode(900, 600), "RPGEngine", sf::Style::Resize | sf::Style::Close);
    ImGui::SFML::Init(*m_window);
    return true;
}

void MakerGUI::eventManager()
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
            signalClose.trigger();
            exit(EXIT_SUCCESS);
        }
        if (event.type == sf::Event::KeyPressed)
        {
            switch (event.key.code)
            {
            case sf::Keyboard::N:
                if (event.key.control)
                {
                    m_ui.newGame.state = UI::NewGame::DIRECTORY;
                }
                else
                {

                }
                break;
            case sf::Keyboard::O:
                if (event.key.control)
                {
                    if (!m_ui.openGame.window)
                    {
                        m_ui.openGame.window = true;
                    }
                }
                else
                {

                }
                break;
            case sf::Keyboard::Q:
                if (event.key.control)
                {
                    signalClose.trigger();
                    exit(EXIT_SUCCESS);
                }
                break;
            }
        }
        if (event.type == sf::Event::KeyReleased)
        {
            switch (event.key.code) {
            case sf::Keyboard::Escape:
                break;
            }
        }
    }

    makeUI();
}

void MakerGUI::draw()
{
    m_window->clear(sf::Color::White);
    ImGui::SFML::Render(*m_window);
    m_window->display();
}

void MakerGUI::doNewCharacter()
{
    m_ui.character.newOne = true;
    m_ui.character.edit = m_ui.character.current = Maker::CharacterInformations();
    strcpy(m_ui.character.name, "");
    m_ui.character.currentCharacter = -1;
}

void MakerGUI::doDeleteCharacter()
{
    if (!m_ui.character.current.name.empty())
        m_maker->deleteCharacter(m_ui.character.current.name);
    doNewCharacter();
}

void MakerGUI::makeUI()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New", "Ctrl+N"))
            {
                m_ui.newGame.state = UI::NewGame::DIRECTORY;

            }
            if (ImGui::MenuItem("Open", "Ctrl+O"))
            {
                m_ui.openGame.window = true;
            }
            if (ImGui::MenuItem("Save", "Ctrl+S"))
            {

            }
            if (ImGui::MenuItem("Save As", "Ctrl+Shift+S"))
            {

            }
            ImGui::Separator();
            if (ImGui::MenuItem("Quit", "Ctrl+Q"))
            {
                signalClose.trigger();
                exit(EXIT_SUCCESS);
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Display"))
        {
            ImGui::Checkbox("Console", &m_ui.windows.console);
            ImGui::Checkbox("Character", &m_ui.windows.character);

            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
    if (m_ui.windows.console)
    {
        if (ImGui::Begin("Console", nullptr, ImGuiWindowFlags_HorizontalScrollbar))
    {
        bool scrollDown = false;
        if (Logger::newLogs())
            scrollDown = true;
        ImGui::TextUnformatted(Logger::getLog().c_str());
        if (scrollDown)
            ImGui::SetScrollHereY(1.0f);
    }
        ImGui::End();
    }
    if (m_ui.newGame.state == UI::NewGame::DIRECTORY)
    {
        if (!m_fileBrowser)
        {
            m_fileBrowser = std::make_unique<ImGui::FileBrowser>(ImGuiFileBrowserFlags_CreateNewDir |
                                                                 ImGuiFileBrowserFlags_SelectDirectory |
                                                                 ImGuiFileBrowserFlags_CloseOnEsc);
            m_fileBrowser->SetTitle("New game : select the directory");
            m_fileBrowser->Open();
        }
        m_fileBrowser->Display();

        if (m_fileBrowser->HasSelected())
        {
            m_ui.newGame.directory = m_fileBrowser->GetSelected().string();
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

    if (ImGui::BeginPopupModal("New game", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
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
                events::WorkerThread::newWork(m_maker, &Maker::doNewGame, m_ui.newGame.gameName, m_ui.newGame.directory);
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
        }
        if (ImGui::Button("Cancel"))
        {
            m_ui.newGame.state = UI::NewGame::NONE;
            ImGui::CloseCurrentPopup();
        }
        if (ImGui::IsRootWindowOrAnyChildFocused() && !ImGui::IsAnyItemActive() && !ImGui::IsMouseClicked(0))
           ImGui::SetKeyboardFocusHere(0);
        ImGui::EndPopup();
    }

    if (m_ui.openGame.window)
    {
        if (ImGui::Begin("Open Game", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
        {
            if (m_ui.openGame.gameList.size() == 0)
            {
                for (auto& g : m_maker->gameList())
                    m_ui.openGame.gameList.push_back(g);
                m_ui.openGame.selectedItem = 0;
            }
            if (m_ui.openGame.gameList.size() > 0)
            {
                ImGui::ListBox("Select the game", &m_ui.openGame.selectedItem, m_ui.openGame.gameList.data(),
                        m_ui.openGame.gameList.size());

                if (ImGui::Button("OK"))
                {
                    events::WorkerThread::newWork(m_maker, &Maker::doOpenGame, m_ui.openGame.gameList.getStr(m_ui.openGame.selectedItem));
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

    if (m_maker->getStates().progression >= Maker::States::READY)
    {
        if (m_ui.windows.character)
        {
            if (ImGui::Begin("Character", nullptr))
            {
                ImGui::Columns(2);
                if (ImGui::ListBox("", &m_ui.character.currentCharacter, m_ui.character.list.data(), m_ui.character.list.size()))
                {
                    if (!m_maker->getCharacterInformations(m_ui.character.list.getStr(m_ui.character.currentCharacter), m_ui.character.current))
                    {
                        LOG(ERROR) << "Error while loading " << m_ui.character.list.getStr(m_ui.character.currentCharacter) << " character informations";
                        doDeleteCharacter();
                        doNewCharacter();
                    }
                    else
                    {
                        m_ui.character.edit = m_ui.character.current;
                        strcpy(m_ui.character.name, m_ui.character.edit.name.c_str());
                        m_ui.character.newOne = false;
                    }
                }

                ImGui::NextColumn();
                ImGui::InputText("Name", m_ui.character.name, 16);
                ImGui::RadioButton("NPC", (int*)&m_ui.character.edit.type, Maker::CharacterInformations::NPC);
                ImGui::RadioButton("Vendor", (int*)&m_ui.character.edit.type, Maker::CharacterInformations::VENDOR);

                if (strlen(m_ui.character.name) > 0)
                {
                    if (ImGui::Button("Save"))
                    {
                        m_ui.character.edit.name = m_ui.character.name;
                        if (m_ui.character.newOne)
                        {
                            m_maker->saveCharacter(m_ui.character.edit);
                            m_ui.character.newOne = false;
                        }
                        else
                            m_maker->saveCharacter(m_ui.character.edit, m_ui.character.current);
                        m_ui.character.current = m_ui.character.edit;
                    }
                }
                ImGui::Columns();
                if (ImGui::Button("New"))
                {
                    doNewCharacter();
                }
                ImGui::SameLine();
                if (ImGui::Button("Delete"))
                {
                    doDeleteCharacter();
                }
            }
            ImGui::End();
        }
    }
}



} // namespace maker::GUI
