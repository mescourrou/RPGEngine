#include "GameGUI.hpp"

// Project
#include <Model.hpp>
#include <Database.hpp>
#include <VerbosityLevels.hpp>
#include <Query.hpp>
#include <Config.hpp>
#include <ConfigFiles.hpp>
#include <general_config.hpp>
#include <Game.hpp>
#include <Character.hpp>

// External lib
#include <glog/logging.h>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

namespace game {

namespace GUI {

/**
 * @brief Construct the GameGUI
 * @param context Context to use
 */
GameGUI::GameGUI(std::shared_ptr<config::Context> context, Game* game):
    m_context(context), m_game(game)
{
    VLOG(verbosityLevel::OBJECT_CREATION) << "Creating " << className() << " => " << this;
    namespace structure = config::structure::globalFile;
    bool fullscreen = false;
    if (m_context->config()->getValue(structure::preferences::SECTION, structure::preferences::FULLSCREEN) == "true")
        fullscreen = true;

    std::string strResolution = m_context->config()->getValue(structure::preferences::SECTION, structure::preferences::RESOLUTION);

    if (strResolution.empty())
        strResolution = GAME_DEFAULT_RESOLUTION;

    auto xIndex = strResolution.find('x');
    int xResolution = std::atoi(strResolution.substr(0,xIndex).c_str());
    int yResolution = std::atoi(strResolution.substr(xIndex+1,-1).c_str());

    VLOG(verbosityLevel::VERIFICATION_LOG) << "Resolution = " << xResolution << " x " << yResolution;

    if (!fullscreen)
        m_window = std::make_shared<sf::RenderWindow>(sf::VideoMode(xResolution, yResolution), "RPGEngine");
    else
        m_window = std::make_shared<sf::RenderWindow>(sf::VideoMode(xResolution, yResolution), "RPGEngine", sf::Style::Fullscreen);
}

/**
 * @brief Initialize the GUI part of Game
 * @param db Database to use
 * @return Return true if the initialization went well
 */
bool GameGUI::initialize(std::shared_ptr<database::Database> db)
{
    VLOG(verbosityLevel::FUNCTION_CALL) << "Initialize";
    namespace Model = database::Model::Game;
    using namespace database;
    if (!db)
        throw GameGUIException("No database given.", DatabaseException::MISSING_DATABASE);

    auto result = db->query(Query::createQuery<Query::SELECT>(Model::TABLE, db).column(Model::FIRST_MAP_NAME));
    if (result.size() == 0)
        return false;

    LOG(INFO) << "Load first map";
    m_map = std::make_shared<map::Map>(m_context, result.at(1).at(Model::FIRST_MAP_NAME));
    if (!m_map->load(result.at(1).at(Model::FIRST_MAP_NAME)))
    {
        LOG(ERROR) << "Error during loading the map";
        return false;
    }
    return true;
}

/**
 * @brief Manage the SFML events
 */
void GameGUI::eventManager()
{
    static map::Vector<2> position;
    map::Vector<2> moveVector;
    // Process events
    sf::Event event;
    while (m_window->pollEvent(event))
    {
        // Close window: exit
        if (event.type == sf::Event::Closed)
        {
            m_window->close();
            m_signalOnClose.trigger();
        }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        moveVector += {-10, 0};
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        moveVector += {10, 0};
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        moveVector += {0, -10};
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        moveVector += {0, 10};
    if (moveVector != map::Vector<2>{0,0})
    {
        moveVector.x() = moveVector.x() / moveVector.norm() * 10;
        moveVector.y() = moveVector.y() / moveVector.norm() * 10;
        map::Vector<2> intersection;
        if (!m_map->collision(position, moveVector, intersection))
        {
            position += moveVector;
            m_map->setCenterOfView(position);
        }
        else
        {
            if (intersection != map::Vector<2>{-1, -1})
            {
                if (moveVector.x() > 0)
                    intersection.x() -= 1;
                else if (moveVector.x() < 0)
                    intersection.x() += 1;
                if (moveVector.y() > 0)
                    intersection.y() -= 1;
                else if (moveVector.y() < 0)
                    intersection.y() += 1;
                position = intersection;
                m_map->setCenterOfView(position);
            }
        }
    }
}

/**
 * @brief Draw the window and the widgets
 */
void GameGUI::draw()
{
    m_window->clear();
    m_window->draw(*m_map);
    m_window->draw(*m_game->m_playerCharacter);
    m_window->display();
}

} // namespace GUI

} // namespace map
