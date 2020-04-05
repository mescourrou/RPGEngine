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
#include <WindowsManager.hpp>
#include <CharacterWindow.hpp>
#include <InventoryWindow.hpp>

#include <SFML/Window/Event.hpp>
#include <imgui.h>

namespace sf
{
class RenderWindow;
}

namespace character::gui
{
class CharacterGUI;
}


namespace game
{

class Game;

namespace gui
{

CREATE_EXCEPTION_CLASS(GameGUI)

/**
 * @brief Game GUI manager
 */
class GameGUI : public BaseObject
{
    DECLARE_BASEOBJECT(GameGUI)
  public:
    GameGUI(std::shared_ptr<config::Context> context, Game* game);
    ~GameGUI() override;

    bool initialize(std::shared_ptr<databaseTools::Database> db);
    void eventManager();

    void draw();

    /// Signal when a key is pressed
    events::Event<sf::Event::KeyEvent> signalKeyPressed;
    /// Signal when a key is released
    events::Event<sf::Event::KeyEvent> signalKeyReleased;
    /// Signal when a arrow is pressed (no security to get only one event)
    events::Event<sf::Keyboard::Key> signalArroyIsPressed;
    /// Signal when the pause is activated or not
    events::Event<bool> signalPause;
    /**
     * @brief Get the event triggered when the user close the game
     */
    void subscribeOnClose(const std::function<void(void)>& func)
    {
        m_signalOnClose.subscribeAsync(func);
    }

    /**
     * @brief Add a BaseGUIObject to the list
     * @param args Arguments needed to create the BaseGUIObject
     * @return Weak_ptr on the created object
     */
    template<typename BaseGUIObject_T, typename... Args, typename = std::enable_if<std::is_base_of_v<BaseGUIObject, BaseGUIObject_T>>>
             std::weak_ptr<BaseGUIObject_T> addGUIObject(Args... args)
    {
        return std::dynamic_pointer_cast<BaseGUIObject_T>(m_guiObjects.emplace_back(
                    std::make_shared<BaseGUIObject_T>(args...)));
    }

  protected:
    void loadFromConfig();

    void makeUI();
    void uiPauseMenu();
    void uiLoadSettingsPopup();
    void uiInformationPopup();
    void uiSettingsPopup();
    void managePressingKeyEvent(const sf::Event::KeyEvent& key);
    void manageReleasingKeyEven(const sf::Event::KeyEvent& key);
    void checkKeyPressed();
  private:
    /// List of BaseGUIObjects to manage and draw
    std::vector<std::shared_ptr<BaseGUIObject>> m_guiObjects;
    /// Pointer on the GUI object linked to the player
    std::weak_ptr<character::gui::CharacterGUI> m_player;
    /// Current mapGUI
    std::shared_ptr<map::gui::MapGUI> m_mapGUI;
    /// Context to use
    std::shared_ptr<config::Context> m_context;
    /// Database to use
    std::shared_ptr<databaseTools::Database> m_db;
    /// SFML render window
    std::shared_ptr<sf::RenderWindow> m_window;

    /// Event when the user close the game
    events::Event<void> m_signalOnClose;

    /// Window manager
    ImGui::WindowsManager m_windowsManager;
    /// Character window
    std::unique_ptr<CharacterWindow> m_characterWindow;
    /// Character window toggle action identifyer
    static constexpr char CHARACTER_WINDOW_ACTION[] = "Toggle character window";
    std::unique_ptr<InventoryWindow> m_inventoryWindow; ///< Inventory window
    /// Inventory window toggle action identifyer
    static constexpr char INVENTORY_WINDOW_ACTION[] = "Toggle inventory window";

    /// Contains the action name waiting to associate a keybinding
    std::string m_actionWaitingForKeybinding = "";

    /// Event, created once
    sf::Event m_event;

    /// Pointer on the game to facilitate the interaction
    Game* m_game;

    /// Timer of the last window draw
    std::chrono::time_point<std::chrono::system_clock> m_drawingTimer;
    /// Frame period. 60 fps by default
    std::chrono::duration<unsigned int, std::micro>
    m_drawingPeriod =
        std::chrono::duration<unsigned int, std::micro>
        ((unsigned int)(1000000. / 60.)); // 60 fps
    /**
     * @brief Informations necessary for the UI
     */
    struct UI
    {
        static constexpr char MAIN_UI[] = "mainUi";
        static constexpr char PAUSE_POPUP[] = "Pause";
        static constexpr char INFOS_POPUP[] = "Infos";

        static constexpr char SETTINGS_POPUP[] = "Settings";
        static constexpr char SETTINGS_TABBAR_NAME[] = "TabBar";
        static constexpr char BOTTON_AREA[] = "##Character";
        static constexpr char CHARACTER_BUTTON[] = "Character";
        static constexpr char INVENTORY_BUTTON[] = "Inventory";
        static constexpr ImGuiWindowFlags FIXED = ImGuiWindowFlags_NoMove |
                ImGuiWindowFlags_NoResize;
        /// If the system is on pause
        bool onPause = false;
        /// If the global ui is activated
        bool uiActivated = true;
        /// If the inventory window is open
        bool inventoryOpen = false;
        /// If the character window is open
        bool characterOpen = false;

        /**
         * @brief Settings selected
         */
        struct Settings
        {
            /// Is fullscreen on ?
            bool fullscreen = false;
            /// Currrent resolution
            std::string resolution = "";
            /// Selected resolution index
            int resolutionItemSelected = 0;
            /// Available resolutions
            std::vector<const char*> availableResolutions;
        };
        /// Seleted settings
        Settings settings;
    };
    /// UI structure containing the ui linked variable
    UI m_ui;


};

} // namespace gui

} // namespace map
