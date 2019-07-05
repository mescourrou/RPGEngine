#include "MapGUI.hpp"
#include <tinyxml2.h>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include <glog/logging.h>

namespace map {

namespace GUI {

map::GUI::MapGUI::MapGUI(std::shared_ptr<config::Context> context, const std::string& name) :
    Map(context, name)
{

}

void MapGUI::move(double offsetX, double offsetY)
{
    m_centerOfView += Vector<2>{offsetX, offsetY};
    if (m_centerOfView.x() < 0)
        m_centerOfView.x() = 0;
    if (m_centerOfView.y() < 0)
        m_centerOfView.y() = 0;

    if (m_centerOfView.x() >= (m_width-1)*m_tileWidth)
        m_centerOfView.x() = (m_width-1)*m_tileWidth;
    if (m_centerOfView.y() >= (m_height-1)*m_tileHeight)
        m_centerOfView.y() = (m_height-1)*m_tileHeight;
}

void map::GUI::MapGUI::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    // Position on the map of the top left corner of the screen
    // Unit : pixels
    sf::Vector2f topLeftPosition(m_centerOfView.x() - target.getSize().x / 2,
                                 m_centerOfView.y() - target.getSize().y / 2);

    // Coordinates of the tile of the top left corner
    int i = topLeftPosition.x / m_tileWidth;
    int j = topLeftPosition.y / m_tileHeight;

    sf::Vector2f origin;
    // Position on the screen of the top left displayed tile
    // It's negative to cover all the screen
    origin.x = - (static_cast<int>(m_tileWidth) - static_cast<int>(m_centerOfView.x()) % static_cast<int>(m_tileWidth));
    origin.y = - (static_cast<int>(m_tileHeight) - static_cast<int>(m_centerOfView.y()) % static_cast<int>(m_tileHeight));

    // Copy to iterate without modify the origin
    sf::Vector2f tilePosition(origin);

    while (tilePosition.y < target.getSize().y)
    {
        while(tilePosition.x < target.getSize().x)
        {
            if (i >= 0 && j >= 0 && i < m_width && j < m_height)
            {
                sf::Sprite tile(m_tiles.at(m_idMap.at(i).at(j)));
                tile.setPosition(tilePosition);
                target.draw(tile, states);
            }

            tilePosition.x += m_tileWidth;
            i++;
        }
        tilePosition.x = origin.x;
        tilePosition.y += m_tileHeight;
        i = topLeftPosition.x / m_tileWidth;
        j++;
    }
}

bool MapGUI::doLoadTiles(const json &layer)
{
    if (!layer.contains(KEY_TILE_DATA))
        return false;
    if (!layer[KEY_TILE_DATA].is_array())
        return false;

    unsigned int i = 0;
    unsigned int j = 0;
    for (auto& id : layer[KEY_TILE_DATA])
    {
        m_idMap[i][j] = id;

        j++;
        if (j >= m_width)
        {
            j = 0;
            i++;
        }
    }
    return true;
}

bool MapGUI::doLoadTilesets(const json &json)
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
        if (!loadTileset(tileset))
            return false;
    }
    return true;
}

bool MapGUI::loadTileset(const json &tileset)
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
    tinyxml2::XMLError err = doc.LoadFile(std::string(m_context->kMapPath() + '/' + tileset[KEY_TILESET_SOURCE].get<std::string>()).c_str());
    if (err)
    {
        LOG(ERROR) << "Error during loading file " << tileset[KEY_TILESET_SOURCE].get<std::string>().c_str();
    }

    auto xmlTileset = doc.FirstChildElement(ELEMENT_TILESET);
    if (!xmlTileset)
    {
        LOG(ERROR) << "No " << ELEMENT_TILESET << " in the Tileset " << m_context->kMapPath() << '/' << tileset[KEY_TILESET_SOURCE].get<std::string>();
        return false;
    }
    if (!xmlTileset->FindAttribute(PROPERTY_TILE_WIDTH))
    {
        LOG(ERROR) << "No " << PROPERTY_TILE_WIDTH << " in the Tileset " << m_context->kMapPath() << '/' << tileset[KEY_TILESET_SOURCE].get<std::string>();
        return false;
    }
    unsigned int width = xmlTileset->FindAttribute(PROPERTY_TILE_WIDTH)->IntValue();

    if (!xmlTileset->FindAttribute(PROPERTY_TILE_HEIGHT))
    {
        LOG(ERROR) << "No " << PROPERTY_TILE_HEIGHT << " in the Tileset " << m_context->kMapPath() << '/' << tileset[KEY_TILESET_SOURCE].get<std::string>();
        return false;
    }
    unsigned int height = xmlTileset->FindAttribute(PROPERTY_TILE_HEIGHT)->IntValue();

    if (!xmlTileset->FindAttribute(PROPERTY_TILE_COUNT))
    {
        LOG(ERROR) << "No " << PROPERTY_TILE_COUNT << " in the Tileset " << m_context->kMapPath() << '/' << tileset[KEY_TILESET_SOURCE].get<std::string>();
        return false;
    }
    unsigned int tilecount = xmlTileset->FindAttribute(PROPERTY_TILE_COUNT)->IntValue();

    if (!xmlTileset->FindAttribute(PROPERTY_TILE_COLUMNS))
    {
        LOG(ERROR) << "No " << PROPERTY_TILE_COLUMNS << " in the Tileset " << m_context->kMapPath() << '/' << tileset[KEY_TILESET_SOURCE].get<std::string>();
        return false;
    }
    unsigned int columns = xmlTileset->FindAttribute(PROPERTY_TILE_COLUMNS)->IntValue();

    auto xmlImage = xmlTileset->FirstChildElement(ELEMENT_IMAGE);
    std::string imageFilename = xmlImage->FindAttribute(PROPERTY_IMAGE_SOURCE)->Value();
    imageFilename = m_context->kMapPath() + '/' + imageFilename;

    sf::Texture texture;
    if (!texture.loadFromFile(imageFilename))
        LOG(WARNING) << "Texture from " << imageFilename << " not loaded";

    for (unsigned int line = 0; ; line++)
    {
        for (unsigned int col = 0; col < columns; col++)
        {
            sf::Sprite tile(texture, sf::IntRect(col*width, line*height, width, height));
            m_tiles.insert(std::pair<unsigned int, sf::Sprite>(id, tile));

            id++;
            if (id >= tilecount)
                break;
        }
        if (id >= tilecount)
            break;
    }


    LOG(INFO) << "Loading map succefully";
    return true;
}

} // namespace GUI

} // namespace map

