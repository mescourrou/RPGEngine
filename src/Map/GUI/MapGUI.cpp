#include "MapGUI.hpp"

// Project
#include <Position.hpp>

// External library
#include <tinyxml2.h>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include <glog/logging.h>

namespace map {

namespace GUI {

/**
 * @brief Destructor
 *
 * Free the textures
 */
MapGUI::~MapGUI()
{
    for (auto ptr : m_textures)
        delete ptr;
}

/**
 * @brief Move the center of the view
 * @param offsetX X move
 * @param offsetY Y move
 */
void MapGUI::move(double offsetX, double offsetY)
{
    m_centerOfView += Vector<2>{offsetX, offsetY};
    saturateCenterOfView();
}

/**
 * @brief m_centerOfView setter
 */
void MapGUI::setCenterOfView(const Position &centralPosition)
{
    m_centerOfView.x() = centralPosition.x();
    m_centerOfView.y() = centralPosition.y();
    saturateCenterOfView();
}

/**
 * @brief Prepare the drawing
 */
void MapGUI::prepare(const sf::Vector2u &targetSize)
{

}

/**
 * @brief Convert the given map position into a pixel position on the screen
 * @param position Position on the map
 * @return Position on the screen
 */
sf::Vector2f MapGUI::positionOnScreenFrom(const Position &position)
{
    return sf::Vector2f(static_cast<float>(position.x()) - m_topLeftPosition.x, static_cast<float>(position.y()) - m_topLeftPosition.y);
}

//void MapGUI::draw(BaseGUIObject *obj)
//{
//    if (obj)
//    {
//        obj->setOnScreenPosition(positionOnScreenFrom(obj-))
//    }
//}

void MapGUI::setTarget(std::weak_ptr<sf::RenderTarget> target, const sf::RenderStates &states)
{
    m_target = target;
    m_states = states;
}

/**
 * @brief Draw the map on the target
 * @param target Target to draw on
 * @param states Render states to use
 */
void map::GUI::MapGUI::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    // Position on the map of the top left corner of the screen
    // Unit : pixels
    m_topLeftPosition.x = m_centerOfView.x() - target.getSize().x / 2;
    m_topLeftPosition.y = m_centerOfView.y() - target.getSize().y / 2;
    // Coordinates of the tile of the top left corner
    int i = std::floor(m_topLeftPosition.x / m_tileWidth);
    int j = std::floor(m_topLeftPosition.y / m_tileHeight);
    int iOrigin = i;

    sf::Vector2f origin;
    // Position on the screen of the top left displayed tile
    // It's negative to cover all the screen
    origin.x = - static_cast<int>(m_centerOfView.x()) % static_cast<int>(m_tileWidth);
    origin.y = - static_cast<int>(m_centerOfView.y()) % static_cast<int>(m_tileHeight);
    // Copy to iterate without modify the origin
    sf::Vector2f tilePosition(origin);

    while (tilePosition.y < target.getSize().y)
    {
        while(tilePosition.x < target.getSize().x)
        {
            if (i >= 0 && j >= 0 && i < m_width && j < m_height)
            {
                unsigned int id = m_idMap.at(i).at(j);
                if (m_tiles.find(id) != m_tiles.end())
                {
                    sf::Sprite tile(m_tiles.at(id));
                    tile.setPosition(tilePosition);
                    target.draw(tile, states);
                }
                else {
                    sf::RectangleShape emptyTile(sf::Vector2f(m_tileWidth, m_tileHeight));
                    emptyTile.setPosition(tilePosition);
                    target.draw(emptyTile, states);
                }
            }

            tilePosition.x += m_tileWidth;
            i++;
        }
        tilePosition.x = origin.x;
        tilePosition.y += m_tileHeight;
        i = iOrigin;
        j++;
    }
}

/**
 * @brief Load the tiles from the json layer
 *
 * We use Tiled to generate the json : https://www.mapeditor.org/
 * @param layer Layer to use
 * @return Return true if all went well
 */
bool MapGUI::loadTiles(const json &layer)
{
    if (!layer.contains(KEY_TILE_DATA))
        return false;
    if (!layer[KEY_TILE_DATA].is_array())
        return false;

    unsigned int i = 0;
    unsigned int j = 0;
    for (auto& id : layer[KEY_TILE_DATA])
    {
        m_idMap[j][i] = id;

        j++;
        if (j >= m_height)
        {
            j = 0;
            i++;
        }
    }
    return true;
}

/**
 * @brief Load the tilesets from the json layer
 *
 * We use Tiled to generate the json : https://www.mapeditor.org/
 * @param json Json object to use
 * @return Return true if all went well
 */
bool MapGUI::loadTilesets(const std::string& mapDirPath, const json &json)
{
    if (!json.contains(KEY_WIDTH) || !json[KEY_WIDTH].is_number_integer())
        return false;
    m_width = json[KEY_WIDTH].get<unsigned int>();
    if (!json.contains(KEY_HEIGHT) || !json[KEY_HEIGHT].is_number_integer())
        return false;
    m_height = json[KEY_HEIGHT].get<unsigned int>();

    if (!json.contains(KEY_TILE_WIDTH) || !json[KEY_TILE_WIDTH].is_number_integer())
        return false;
    m_tileWidth = json[KEY_TILE_WIDTH].get<unsigned int>();
    if (!json.contains(KEY_TILE_HEIGHT) || !json[KEY_TILE_HEIGHT].is_number_integer())
        return false;
    m_tileHeight = json[KEY_TILE_HEIGHT].get<unsigned int>();

    if (!json.contains(KEY_TILESETS) || !json[KEY_TILESETS].is_array())
        return false;
    auto& tilesets = json[KEY_TILESETS];

    for (auto& tileset : tilesets)
    {
        if (!loadTileset(mapDirPath, tileset))
            return false;
    }
    return true;
}

/**
 * @brief Keep the center of view between 0 and the edge of the map
 */
void MapGUI::saturateCenterOfView()
{
    if (m_centerOfView.x() < 0)
        m_centerOfView.x() = 0;
    if (m_centerOfView.y() < 0)
        m_centerOfView.y() = 0;

    if (m_centerOfView.x() >= (m_width-1)*m_tileWidth)
        m_centerOfView.x() = (m_width-1)*m_tileWidth;
    if (m_centerOfView.y() >= (m_height-1)*m_tileHeight)
        m_centerOfView.y() = (m_height-1)*m_tileHeight;
}

/**
 * @brief Load the tileset from the json tileset
 *
 * We use Tiled to generate the json : https://www.mapeditor.org/
 * @param tileset Tileset to use
 * @return Return true if all went well
 */
bool MapGUI::loadTileset(const std::string& mapDirPath, const json &tileset)
{
    VLOG(verbosityLevel::FUNCTION_CALL) << "loadTileset";
    if (!tileset.is_object())
        return false;
    if (!tileset.contains(KEY_TILESET_FIRST_ID) || !tileset[KEY_TILESET_FIRST_ID].is_number_integer())
        return false;
    unsigned int id = tileset[KEY_TILESET_FIRST_ID].get<unsigned int>();

    if (!tileset.contains(KEY_TILESET_SOURCE) || !tileset[KEY_TILESET_SOURCE].is_string())
        return false;

    tinyxml2::XMLDocument doc;
    tinyxml2::XMLError err = doc.LoadFile(std::string(mapDirPath + '/' + tileset[KEY_TILESET_SOURCE].get<std::string>()).c_str());
    if (err)
    {
        LOG(ERROR) << "Error during loading file " << tileset[KEY_TILESET_SOURCE].get<std::string>().c_str();
    }

    auto xmlTileset = doc.FirstChildElement(ELEMENT_TILESET);
    if (!xmlTileset)
    {
        LOG(ERROR) << "No " << ELEMENT_TILESET << " in the Tileset " << mapDirPath << '/' << tileset[KEY_TILESET_SOURCE].get<std::string>();
        return false;
    }
    if (!xmlTileset->FindAttribute(PROPERTY_TILE_WIDTH))
    {
        LOG(ERROR) << "No " << PROPERTY_TILE_WIDTH << " in the Tileset " << mapDirPath << '/' << tileset[KEY_TILESET_SOURCE].get<std::string>();
        return false;
    }
    unsigned int width = xmlTileset->FindAttribute(PROPERTY_TILE_WIDTH)->IntValue();

    if (!xmlTileset->FindAttribute(PROPERTY_TILE_HEIGHT))
    {
        LOG(ERROR) << "No " << PROPERTY_TILE_HEIGHT << " in the Tileset " << mapDirPath << '/' << tileset[KEY_TILESET_SOURCE].get<std::string>();
        return false;
    }
    unsigned int height = xmlTileset->FindAttribute(PROPERTY_TILE_HEIGHT)->IntValue();

    if (!xmlTileset->FindAttribute(PROPERTY_TILE_COUNT))
    {
        LOG(ERROR) << "No " << PROPERTY_TILE_COUNT << " in the Tileset " << mapDirPath << '/' << tileset[KEY_TILESET_SOURCE].get<std::string>();
        return false;
    }
    unsigned int tilecount = xmlTileset->FindAttribute(PROPERTY_TILE_COUNT)->IntValue();

    if (!xmlTileset->FindAttribute(PROPERTY_TILE_COLUMNS))
    {
        LOG(ERROR) << "No " << PROPERTY_TILE_COLUMNS << " in the Tileset " << mapDirPath << '/' << tileset[KEY_TILESET_SOURCE].get<std::string>();
        return false;
    }
    unsigned int columns = xmlTileset->FindAttribute(PROPERTY_TILE_COLUMNS)->IntValue();

    auto xmlImage = xmlTileset->FirstChildElement(ELEMENT_IMAGE);
    std::string imageFilename = xmlImage->FindAttribute(PROPERTY_IMAGE_SOURCE)->Value();
    imageFilename = mapDirPath + '/' + imageFilename;

    m_textures.push_back(new sf::Texture);
    if (!m_textures.back()->loadFromFile(imageFilename))
        LOG(WARNING) << "Texture from " << imageFilename << " not loaded";

    for (unsigned int line = 0; ; line++)
    {
        for (unsigned int col = 0; col < columns; col++)
        {
            sf::Sprite tile(*m_textures.back(), sf::IntRect(col*width, line*height, width, height));
            m_tiles.insert(std::pair<unsigned int, sf::Sprite>((const unsigned int)id, tile));

            id++;
            if (id > tilecount)
                break;
        }
        if (id > tilecount)
            break;
    }


    LOG(INFO) << "Loading map succefully";
    return true;
}

} // namespace GUI

} // namespace map

