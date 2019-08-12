#pragma once

#include <BaseObject.hpp>
#include <BaseException.hpp>
#include <Event.hpp>
#include <Maker.hpp>
#include <Stringlist.hpp>

#include "CharacterWindow.hpp"
#include "ConsoleWindow.hpp"
#include "MoneyWindow.hpp"
#include "MapWindow.hpp"

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <imgui.h>
#include <WindowsManager.hpp>
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
    ~MakerGUI() override;

    bool initialize();
    void eventManager();

    void draw();

    events::Event<void> signalClose;    ///< Signal on closing the app

protected:
    void makeUI();

    sf::RenderWindow m_window;     ///< SFML render window

    std::shared_ptr<config::Context> m_context;     ///< Used context
    std::shared_ptr<map::GUI::MapGUI> m_mapGUI;     ///< Pointer on the GUI map

    /**
     * @brief UI informations
     */
    struct UI {
        /**
         * @brief UI informations for the creation of a new game
         */
        struct NewGame {
            /**
             * @brief State of the creation of a new Game
             */
            enum State {
                NONE,           ///< Nothing
                DIRECTORY,      ///< Selection of the directory
                INFORMATIONS    ///< Getting informations
            };
            State state;                            ///< Current state

            std::string directory = "";             ///< Directory used for the new game
            char gameName[100];                     ///< New game name
        } newGame;                                  ///< UI informations for new game

        /**
         * @brief UI Informations for the openning of the game
         */
        struct OpenGame {
            bool window = false;                    ///< Window openned
            stringlist gameList;                    ///< Game list
            int selectedItem = 0;                   ///< Current game selected
        } openGame;                                 ///< UI informations for openning game window


    } m_ui;                                         ///< UI structure containing the ui linked variable

    void resetUI();

    ImGui::WindowsManager m_windowManager;                              ///< Window manager
    std::unique_ptr<maker::GUI::CharacterWindow> m_characterWindow;     ///< Pointer on the character window
    std::unique_ptr<maker::GUI::ConsoleWindow> m_consoleWindow;         ///< Pointer on the console window
    std::unique_ptr<maker::GUI::MoneyWindow> m_moneyWindow;             ///< Pointer on the money window
    std::unique_ptr<maker::GUI::MapWindow> m_mapWindow;                 ///< Pointer on the map windows

    std::unique_ptr<ImGui::FileBrowser> m_fileBrowser;                  ///< File browser

    Maker* m_maker;                                                     ///< Pointer on the maker backend

};

} // namespace maker::GUI
