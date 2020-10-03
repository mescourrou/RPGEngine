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
#include "DialogueWindow.hpp"

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <imgui.h>
#include <WindowsManager.hpp>
#ifdef BUILD_USE_FILESYSTEM
#include <FileBrowser.hpp>
#endif

namespace sf
{
class RenderWindow;
}

namespace config
{
class Context;
}

namespace maker
{
class Maker;
}

namespace map::gui
{
class MapGUI;
}


namespace maker::gui
{

CREATE_EXCEPTION_CLASS(MakerGUI);

/**
 * @brief Game GUI manager
 */
class MakerGUI : public BaseObject
{
    DECLARE_BASEOBJECT(MakerGUI);
  public:
    MakerGUI(std::shared_ptr<config::Context> context, Maker* maker);
    ~MakerGUI() override;

    MakerGUI(const MakerGUI&) = delete;
    MakerGUI(MakerGUI&&) = delete;
    MakerGUI& operator=(const MakerGUI&) = delete;
    MakerGUI& operator=(MakerGUI&&) = delete;

    bool initialize();
    void eventManager();

    void draw();

    /// Signal on closing the app
    ADD_EVENT(SignalClose, void);

  protected:
    void makeUI();
  private:
    void processKeyPressedEvent(const sf::Event& event) const;
    void processKeyReleasedEvent(const sf::Event& event) const;
    void resetUI();

    void makeMainMenuBarUI();
    void makeNewGameUI();
    void makeOpenGameUI();

    /// SFML render window
    sf::RenderWindow m_window;

    /// Used context
    std::shared_ptr<config::Context> m_context;
    /// Pointer on the GUI map
    std::shared_ptr<map::gui::MapGUI> m_mapGUI;

    /**
     * @brief UI informations
     */
    struct UI
    {
        /**
         * @brief UI informations for the creation of a new game
         */
        struct NewGame
        {
            /**
             * @brief State of the creation of a new Game
             */
            enum State
            {
                NONE,           ///< Nothing
                DIRECTORY,      ///< Selection of the directory
                INFORMATIONS    ///< Getting informations
            };
            /// Current state
            State state;

            std::string directory = "";     ///< Directory used for the new game
            char gameName[100];             ///< New game name
        };
        /// UI informations for new game
        struct NewGame newGame;

        /**
         * @brief UI Informations for the openning of the game
         */
        struct OpenGame
        {
            bool window = false;    ///< Window openned
            stringlist gameList;    ///< Game list
            int selectedItem = 0;   ///< Current game selected
        };
        /// UI informations for openning game window
        struct OpenGame openGame;


    };
    /// UI structure containing the ui linked variable
    struct UI m_ui;

    /// Window manager
    ImGui::WindowsManager m_windowManager;
    /// Pointer on the character window
    std::unique_ptr<maker::gui::CharacterWindow> m_characterWindow;
    /// Pointer on the console window
    std::unique_ptr<maker::gui::ConsoleWindow> m_consoleWindow;
    /// Pointer on the money window
    std::unique_ptr<maker::gui::MoneyWindow> m_moneyWindow;
    /// Pointer on the map window
    std::unique_ptr<maker::gui::MapWindow> m_mapWindow;
    /// Pointer on the dialogue window
    std::unique_ptr<maker::gui::DialogueWindow> m_dialogueWindow;

    /// File browser
    std::unique_ptr<ImGui::FileBrowser> m_fileBrowser;

    /// Pointer on the maker backend
    Maker* m_maker;

};

} // namespace maker::gui
