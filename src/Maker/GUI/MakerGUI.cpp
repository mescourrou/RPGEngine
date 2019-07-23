#include "MakerGUI.hpp"
#include <VerbosityLevels.hpp>
#include <Context.hpp>
#include <WorkerThread.hpp>
#include <Maker.hpp>
#include <Logger.hpp>
#include <Popups.hpp>

#include <MapGUI.hpp>

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

    m_maker->signalMapUdated.subscribeSync([this](std::weak_ptr<map::Map> mapPtr) {
       m_mapGUI = std::make_shared<map::GUI::MapGUI>(mapPtr);
       m_mapGUI->load(m_context->kMapPath());
    });

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
        if (event.type == sf::Event::Resized)
        {
            if (m_mapGUI) m_mapGUI->forcePrepare(m_window->getView().getSize());
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
            case sf::Keyboard::Left:
                if (m_mapGUI) m_mapGUI->move(-5,0);
                break;
            case sf::Keyboard::Right:
                if (m_mapGUI) m_mapGUI->move(5,0);
                break;
            case sf::Keyboard::Up:
                if (m_mapGUI) m_mapGUI->move(0,-5);
                break;
            case sf::Keyboard::Down:
                if (m_mapGUI) m_mapGUI->move(0,5);
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
    if (m_mapGUI)
        m_mapGUI->prepare(m_window->getView().getSize());
    m_window->clear();//sf::Color::White);
    if (m_mapGUI)
    {
        m_window->draw(*m_mapGUI);
    }
    ImGui::Popups::Draw();
    ImGui::SFML::Render(*m_window);
    m_window->display();
}

void MakerGUI::resetUI()
{
    m_ui = UI();
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

void MakerGUI::doSaveMoney()
{
    for (auto& v : m_ui.money.infos.values)
    {
        if (std::count(m_ui.money.infos.values.begin(), m_ui.money.infos.values.end(), v) > 1)
        {
            ImGui::Popups::Error("Multiple money with same values (" + std::to_string(v) + ")");
            return;
        }
    }
    m_ui.money.infos.moneyList = m_ui.money.nameList.toVectorString();
    m_maker->saveMoney(m_ui.money.infos);
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
            ImGui::Checkbox("Money system", &m_ui.windows.money);
            ImGui::Checkbox("Map selector", &m_ui.windows.maps);
            ImGui::Checkbox("Current map", &m_ui.windows.currentMap);

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
            m_ui.newGame.directory.erase(m_ui.newGame.directory.end()-1);
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
                events::WorkerThread::newWork(m_maker, &Maker::doNewGame, std::string(m_ui.newGame.gameName), m_ui.newGame.directory);
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

                m_ui.openGame.gameList = m_maker->gameList();
                m_ui.openGame.selectedItem = 0;
            }
            if (m_ui.openGame.gameList.size() > 0)
            {
                ImGui::ListBox("Select the game", &m_ui.openGame.selectedItem, m_ui.openGame.gameList.data(),
                        m_ui.openGame.gameList.size());

                if (ImGui::Button("OK"))
                {
                    events::WorkerThread::newWork(m_maker, &Maker::doOpenGame, m_ui.openGame.gameList.getStr(m_ui.openGame.selectedItem));
                    m_ui.money.moneyLoaded = false;
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
        if (m_ui.windows.currentMap && m_ui.map.list.size() > 0 && m_ui.windows.character)
        {
            if (ImGui::Begin("Character", nullptr))
            {
                ImGui::Columns(2);
                if (ImGui::ListBox("", &m_ui.character.currentCharacter, m_ui.character.list.data(), m_ui.character.list.size()))
                {
                    if (!m_maker->getCharacterInformations(m_ui.character.list.getStr(m_ui.character.currentCharacter), m_ui.character.current))
                    {
                        ImGui::Popups::Error(std::string("Error while loading " + m_ui.character.list.getStr(m_ui.character.currentCharacter) + " character informations").c_str());
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
                        if (m_ui.map.list.size() == 0)
                            ImGui::Popups::Error("Create a map before creating a character:\n=> The character list here corresponds to the characters on the current map");
                        else
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
        if (m_ui.windows.money)
        {
            if (ImGui::Begin("Money system", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
            {
                if (!m_ui.money.moneyLoaded)
                {
                    if (!m_maker->getMoneyInformations(m_ui.money.infos))
                        LOG(WARNING) << "Fail to load money information";
                    m_ui.money.nameList = m_ui.money.infos.moneyList;
                    m_ui.money.moneyLoaded = true;
                }
                ImGui::Text("Base money : %s", (m_ui.money.nameList.size() > 0 ? m_ui.money.nameList.get(m_ui.money.infos.baseMoney):"None"));
                for (unsigned int i = 0; i < m_ui.money.nameList.size() ; i++)
                {
                    ImGui::PushID(i);
                    ImGui::RadioButton("", &m_ui.money.infos.baseMoney, i);
                    ImGui::SameLine();
                    ImGui::InputText("Name", m_ui.money.nameList.get(i), 16, ImGuiInputTextFlags_CharsNoBlank);

                    ImGui::SameLine();
                    if (m_ui.money.infos.baseMoney != i)
                    {
                        if (ImGui::InputInt("Value", (int*)&m_ui.money.infos.values.at(i)) && m_ui.money.infos.values.at(i) <= 0)
                            m_ui.money.infos.values.at(i) = 1;
                    }
                    else
                    {
                        m_ui.money.infos.values.at(i) = 1;
                        ImGui::Text("Value = 1");
                    }
                    ImGui::PopID();
                }
                if (ImGui::Button("Save"))
                {
                    doSaveMoney();
                }
                ImGui::SameLine();
                if (ImGui::Button("New money"))
                {
                    m_ui.money.nameList.push_back("");
                    m_ui.money.infos.values.push_back(1);
                }
            }
            ImGui::End();
        }
        if (m_ui.windows.maps)
        {
            if (ImGui::Begin("Map selector"))
            {
                if (!m_ui.map.loaded)
                {
                    m_ui.map.list = m_maker->getMapList();
                    if (m_ui.map.list.size() > 0)
                        m_maker->setCurrentMap(m_ui.map.list.getStr(0));
                    m_ui.map.loaded = true;
                }
                for (unsigned int i = 0; i < m_ui.map.list.size(); i++)
                {
                    ImGui::PushID(i);
                    if (ImGui::RadioButton("##push", (int*)&m_ui.map.selected, i))
                        m_maker->setCurrentMap(m_ui.map.list.getStr(i));
                    ImGui::SameLine();
                    ImGui::InputText("##input", m_ui.map.list.get(i), 16);
                    ImGui::PopID();
                }
                if (ImGui::Button("New map"))
                {
                    m_ui.map.list.push_back("");
                }
                ImGui::SameLine();
                if (ImGui::Button("Delete map"))
                {
                    m_ui.map.list.remove(m_ui.map.selected);
                    if (m_ui.map.selected > 0)
                        m_ui.map.selected--;

                }
            }
            ImGui::End();
        }
        if (m_ui.windows.currentMap && m_ui.map.list.size() > 0)
        {
            if (ImGui::Begin("Current map"))
            {
                ImGui::InputText("Name", m_ui.map.list.get(m_ui.map.selected), 16);

                if (ImGui::Button("Save"))
                {

                }
            }
            ImGui::End();
        }
    }
}



} // namespace maker::GUI
