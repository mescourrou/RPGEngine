#pragma once

// Stl
#include <memory>

// Project
#include <BaseException.hpp>
#include <BaseObject.hpp>
#include <Context.hpp>
#include <MapGUI.hpp>
#include <Event.hpp>
#include <Database.hpp>

#include <SFML/Window/Event.hpp>
#include <imgui.h>

namespace sf {
class RenderWindow;
}

namespace character::GUI {
class CharacterGUI;
}


namespace game {

class Game;

namespace GUI {

CREATE_EXCEPTION_CLASS(GameGUI)

/**
 * @brief Game GUI manager
 */
class GameGUI : public BaseObject
{
    DECLARE_BASEOBJECT(GameGUI)
public:
    GameGUI(std::shared_ptr<config::Context> context, Game *game);
    ~GameGUI() override = default;

    bool initialize(std::shared_ptr<database::Database> db);
    void eventManager();

    void draw();

    events::Event<sf::Event::KeyEvent> signalKeyPressed;    ///< Signal when a key is pressed
    events::Event<sf::Event::KeyEvent> signalKeyReleased;   ///< Signal when a key is released
    events::Event<sf::Keyboard::Key> signalArroyIsPressed;  ///< Signal when a arrow is pressed (no security to get only one event)
    events::Event<bool> signalPause;                        ///< Signal when the pause is activated or not
    /**
     * @brief Get the event triggered when the user close the game
     */
    void subscribeOnClose(std::function<void(void)> func) { m_signalOnClose.subscribeSync(func); }

    /**
     * @brief Add a BaseGUIObject to the list
     * @param args Arguments needed to create the BaseGUIObject
     * @return Weak_ptr on the created object
     */
    template<typename BaseGUIObject_T, typename... Args, typename = std::enable_if<std::is_base_of_v<BaseGUIObject, BaseGUIObject_T>>>
    std::weak_ptr<BaseGUIObject_T> addGUIObject(Args... args)
    {
        return std::dynamic_pointer_cast<BaseGUIObject_T>(m_guiObjects.emplace_back(std::make_shared<BaseGUIObject_T>(args...)));
    }

protected:
    std::vector<std::shared_ptr<BaseGUIObject>> m_guiObjects;   ///< List of BaseGUIObjects to manage and draw
    std::weak_ptr<character::GUI::CharacterGUI> m_player;       ///< Pointer on the GUI object linked to the player

    std::shared_ptr<map::GUI::MapGUI> m_mapGUI;     ///< Current mapGUI

    std::shared_ptr<config::Context> m_context;     ///< Context to use

    std::shared_ptr<database::Database> m_db;       ///< Database to use

    std::shared_ptr<sf::RenderWindow> m_window;     ///< SFML render window

    events::Event<void> m_signalOnClose;             ///< Event when the user close the game

    Game* m_game;                                   ///< Pointer on the game to facilitate the interaction

    struct UI {
        static constexpr char MAIN_UI[] = "mainUi";
        static constexpr char PAUSE_POPUP[] = "Pause";
        static constexpr char INFOS_POPUP[] = "Infos";

        static constexpr char SETTINGS_POPUP[] = "Settings";
        static constexpr char SETTINGS_TABBAR_NAME[] = "TabBar";
        static constexpr char BOTTON_AREA[] = "##Character";
        static constexpr char CHARACTER_BUTTON[] = "Character";
        static constexpr char INVENTORY_BUTTON[] = "Inventory";
        static constexpr ImGuiWindowFlags FIXED = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;
        bool onPause = false;                       ///< If the system is on pause
        bool uiActivated = true;                    ///< If the global ui is activated
        bool inventoryOpen = false;                 ///< If the inventory window is open
        bool characterOpen = false;                 ///< If the character window is open

        struct Settings {
            bool fullscreen = false;
            std::string resolution = "";
            int resolutionItemSelected = 0;
            std::vector<const char*> availableResolutions;
        } settings;
    } m_ui;                                         ///< UI structure containing the ui linked variable

    void makeUI();
    void pauseMenu();
    void loadSettingsPopup();

private:

};

} // namespace GUI

} // namespace map
