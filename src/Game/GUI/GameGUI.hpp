#pragma once

#include <memory>

#include <BaseException.hpp>
#include <BaseObject.hpp>
#include <Context.hpp>

#include <MapGUI.hpp>

#include <SFML/Graphics/RenderWindow.hpp>

namespace game {

class Game;

namespace GUI {

CREATE_EXCEPTION_CLASS(GameGUI)

class GameGUI : public BaseObject
{
    DECLARE_BASEOBJECT(GameGUI)
public:
    GameGUI(std::shared_ptr<config::Context> context);
    ~GameGUI() override = default;

    bool initialize(std::shared_ptr<database::Database> db);
    void eventManager();

    void draw();

    bool createDatabaseModel(std::shared_ptr<database::Database> db);

    void setOnClose(std::function<void(void)> cb) { m_cbOnClose = cb; }

protected:
    bool verifyDatabaseModel(std::shared_ptr<database::Database> db);

    std::shared_ptr<config::Context> m_context;
    std::shared_ptr<map::GUI::MapGUI> m_map;

    std::shared_ptr<database::Database> m_db;

    std::shared_ptr<sf::RenderWindow> m_window;

    std::function<void(void)> m_cbOnClose;
private:

};

} // namespace GUI

} // namespace map
