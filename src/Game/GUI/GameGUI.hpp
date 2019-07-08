#pragma once

// Stl
#include <memory>

// Project
#include <BaseException.hpp>
#include <BaseObject.hpp>
#include <Context.hpp>
#include <Map.hpp>
#include <Event.hpp>

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
    events::Event<sf::Event::KeyEvent>& signalKeyPressed() { return m_signalKeyPressed; }

    /**
     * @brief Get the event triggered when the user close the game
     */
    void subscribeOnClose(std::function<void(void)> func) { m_signalOnClose.subscribeSync(func); }

protected:

    std::shared_ptr<config::Context> m_context;     ///< Context to use

    std::shared_ptr<database::Database> m_db;       ///< Database to use

    std::shared_ptr<sf::RenderWindow> m_window;     ///< SFML render window

    events::Event<void> m_signalOnClose;             ///< Event when the user close the game
    events::Event<sf::Event::KeyEvent> m_signalKeyPressed;

    Game* m_game;
private:

};

} // namespace GUI

} // namespace map
