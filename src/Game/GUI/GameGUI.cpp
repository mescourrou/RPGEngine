#include "GameGUI.hpp"

#include <Model.hpp>
#include <Database.hpp>
#include <glog/logging.h>
#include <VerbosityLevels.hpp>
#include <Query.hpp>
#include <Config.hpp>
#include <ConfigFiles.hpp>

#include <general_config.hpp>

#include <SFML/Window/Event.hpp>

namespace game {

namespace GUI {

GameGUI::GameGUI(std::shared_ptr<config::Context> context):
    m_context(context)
{
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
    m_map = std::make_shared<map::GUI::MapGUI>(m_context, result.at(1).at(Model::FIRST_MAP_NAME));
    if (!m_map->load(result.at(1).at(Model::FIRST_MAP_NAME)))
    {
        LOG(ERROR) << "Error during loading the map";
        return false;
    }
    return true;
}

void GameGUI::eventManager()
{
    // Process events
    sf::Event event;
    while (m_window->pollEvent(event))
    {
        // Close window: exit
        if (event.type == sf::Event::Closed)
        {
            m_window->close();
            m_cbOnClose();
        }
        if (event.type == sf::Event::KeyPressed)
        {
            switch (event.key.code)
            {
            case sf::Keyboard::Left:
                m_map->move(-10, 0);
                break;
            case sf::Keyboard::Right:
                m_map->move(10, 0);
                break;
            case sf::Keyboard::Up:
                m_map->move(0, -10);
                break;
            case sf::Keyboard::Down:
                m_map->move(0, 10);
                break;
            default:
                break;
            }
        }
    }
}

void GameGUI::draw()
{
    m_window->clear();
    m_window->draw(*m_map);
    m_window->display();
}



} // namespace GUI

} // namespace map
