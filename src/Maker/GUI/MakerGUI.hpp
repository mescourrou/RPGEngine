#pragma once

#include <BaseObject.hpp>
#include <BaseException.hpp>
#include <Event.hpp>
#include <Maker.hpp>
#include <Stringlist.hpp>

#include <SFML/Window/Event.hpp>
#include <imgui.h>
#ifdef BUILD_USE_FILESYSTEM
#include <FileBrowser.hpp>
#endif

namespace sf {
class RenderWindow;
}

namespace config {
class Context;
}

namespace maker {
class Maker;
}

namespace map::GUI {
class MapGUI;
}


namespace maker::GUI {

CREATE_EXCEPTION_CLASS(MakerGUI)

/**
 * @brief Game GUI manager
 */
class MakerGUI : public BaseObject
{
    DECLARE_BASEOBJECT(MakerGUI)
public:
    MakerGUI(std::shared_ptr<config::Context> context, Maker* maker);
    ~MakerGUI() override = default;

    bool initialize();
    void eventManager();

    void draw();

    events::Event<void> signalClose;

protected:
    std::shared_ptr<sf::RenderWindow> m_window;     ///< SFML render window

    std::shared_ptr<config::Context> m_context;
    std::shared_ptr<map::GUI::MapGUI> m_mapGUI;

    struct UI {
        struct NewGame {
            enum State {
                NONE,
                DIRECTORY,
                INFORMATIONS
            };
            State state;

            std::string directory = "";
            char gameName[100];
        } newGame;
        struct OpenGame {
            bool window = false;
            stringlist gameList;
            int selectedItem = 0;
        } openGame;

        struct Character {
            stringlist list;
            int currentCharacter = -1;
            Maker::CharacterInformations current;
            Maker::CharacterInformations edit;
            char name[16];
            bool newOne = true;
        } character;

        struct Money {
            Maker::MoneyInformations infos;
            stringlist nameList;
            bool moneyLoaded = false;
        } money;

        struct Map {
            stringlist list;
            unsigned int selected = 0;
            bool loaded = false;
        } map;

        struct {
            bool console = true;
            bool character = true;
            bool money = true;
            bool maps = true;
            bool currentMap = true;
        } windows;

        struct Popups {
            struct pair{
                std::string message = "";
                bool open = false;
            };

            pair error;
        } popups;

    } m_ui;                                         ///< UI structure containing the ui linked variable

    void resetUI();

    void doNewCharacter();
    void doDeleteCharacter();

    void doSaveMoney();

    std::unique_ptr<ImGui::FileBrowser> m_fileBrowser;
    void makeUI();

    Maker* m_maker;

};

} // namespace maker::GUI
