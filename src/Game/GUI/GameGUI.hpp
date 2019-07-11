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

namespace sf {
class RenderWindow;
}


namespace game {

class Game;

namespace GUI {

CREATE_EXCEPTION_CLASS(GameGUI)

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
    events::Event<sf::Keyboard::Key> signalArroyIsPressed;
    /**
     * @brief Get the event triggered when the user close the game
     */
    void subscribeOnClose(std::function<void(void)> func) { m_signalOnClose.subscribeSync(func); }

    template<typename BaseGUIObject_T, typename... Args, typename = std::enable_if<std::is_base_of_v<BaseGUIObject, BaseGUIObject_T>>>
    std::weak_ptr<BaseGUIObject_T> addGUIObject(Args... args)
    {
        return std::dynamic_pointer_cast<BaseGUIObject_T>(m_guiObjects.emplace_back(std::make_shared<BaseGUIObject_T>(args...)));
    }

protected:
    std::vector<std::shared_ptr<BaseGUIObject>> m_guiObjects;

    std::shared_ptr<map::GUI::MapGUI> m_mapGUI;

    std::shared_ptr<config::Context> m_context;     ///< Context to use

    std::shared_ptr<database::Database> m_db;       ///< Database to use

    std::shared_ptr<sf::RenderWindow> m_window;     ///< SFML render window

    events::Event<void> m_signalOnClose;             ///< Event when the user close the game

    Game* m_game;                                   ///< Pointer on the game to facilitate the interaction
private:

};

} // namespace GUI

} // namespace map
