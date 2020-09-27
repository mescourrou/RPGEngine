#include "MapGUI.hpp"

// Stl
#include <fstream>

// Project
#include <Position.hpp>
#include <ConfigFiles.hpp>
#include <Tools.hpp>
#include <PerformanceTimer.hpp>
#include <InstrumentationTimer.hpp>

// External library
#include <tinyxml2.h>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include <glog/logging.h>

namespace map
{

namespace gui
{

/**
 * @brief Destructor
 *
 * Free the textures
 */
MapGUI::MapGUI(std::weak_ptr<Map> map) :
    m_map(map)
{

}

/**
 * @brief Move the center of the view
 * @param offsetX X move
 * @param offsetY Y move
 */
void MapGUI::moveCenterOfView(double offsetX, double offsetY)
{
    PROFILE_FUNCTION();
    m_centerOfView.x() += offsetX;
    m_centerOfView.y() += offsetY;
    saturateCenterOfView();
    m_mapMoved = true;
}

/**
 * @brief m_centerOfView setter
 */
void MapGUI::setCenterOfView(const Position& centralPosition)
{
    PROFILE_FUNCTION();
    m_centerOfView.x() = centralPosition.x();
    m_centerOfView.y() = centralPosition.y();
    saturateCenterOfView();
    m_mapMoved = true;
}

/**
 * @brief Load the GUI elements from the given directory
 * @param mapDirPath Directory where are the map elements
 * @return Return true if all went well
 */
bool MapGUI::load(const std::string& mapDirPath)
{
    PROFILE_FUNCTION();
    std::ifstream file(mapDirPath + "/" + Tools::snakeCase(m_map.lock()->name()) +
                       ".json");
    if (file.is_open())
    {
        json json;
        file >> json;
        if (!json.is_object())
            return false;
        namespace mapFile = config::structure::mapFile;
        if (!loadTilesets(mapDirPath, json))
            return false;
        for (auto layer : json[mapFile::KEY_LAYERS])
        {
            if (layer[mapFile::KEY_LAYER_TYPE] == mapFile::TYPE_DATA_LAYER
                    && !loadTiles(layer))
                return false;
        }
        return true;
    }

    return false;
}

/**
 * @brief Prepare the drawing
 */
void MapGUI::prepare(const sf::Vector2f& targetSize)
{
    PROFILE_FUNCTION();
    if (m_mapMoved) // We compute the parameters only if the map moved since last time
    {
        // Position on the map of the top left corner of the screen
        // Unit : pixels
        m_topLeftPosition.x = m_centerOfView.x() - targetSize.x / 2.0;
        m_topLeftPosition.y = m_centerOfView.y() - targetSize.y / 2.0;
        // Coordinates of the tile of the top left corner
        m_firstTileCoordinates.x = std::floor(m_topLeftPosition.x /
                                              m_chunksWidthPixels);
        m_firstTileCoordinates.y = std::floor(m_topLeftPosition.y /
                                              m_chunksHeightPixels);

        // Position on the screen of the top left displayed tile
        m_origin.x = - Tools::linearModulo(m_topLeftPosition.x,
                                           static_cast<float>(m_chunksWidthPixels));
        m_origin.y = - Tools::linearModulo(m_topLeftPosition.y,
                                           static_cast<float>(m_chunksHeightPixels));

        m_mapMoved = false;
    }
}

/**
 * @brief Force the preparation of the map even if it didn't moved
 * @param targetSize Size of the target
 */
void MapGUI::forcePrepare(const sf::Vector2f& targetSize)
{
    PROFILE_FUNCTION();
    m_mapMoved = true;
    prepare(targetSize);
}

/**
 * @brief Convert the given map position into a pixel position on the screen
 * @param position Position on the map
 * @return Position on the screen
 */
sf::Vector2f MapGUI::positionOnScreenFrom(const Position& position)
{
    return sf::Vector2f(static_cast<float>(position.x()) - m_topLeftPosition.x,
                        static_cast<float>(position.y()) - m_topLeftPosition.y);
}

/**
 * @brief Draw the map on the target
 * @param target Target to draw on
 * @param states Render states to use
 */
void map::gui::MapGUI::draw(sf::RenderTarget& target,
                            sf::RenderStates states) const
{
    PROFILE_FUNCTION();
    int i = m_firstTileCoordinates.x;
    int j = m_firstTileCoordinates.y;
    // Copy to iterate without modify the origin
    sf::Vector2f tilePosition(m_origin);

    while (tilePosition.y < target.getSize().y)
    {
        while (tilePosition.x < target.getSize().x)
        {
            if (i >= 0 && j >= 0 && i < m_nbChunksWidth && j < m_nbChunksHeight)
            {
                sf::Sprite& tile = m_chunks[j][i];
                tile.setPosition(tilePosition);
                target.draw(tile, states);
            }

            tilePosition.x += m_chunksWidthPixels;
            i++;
        }
        tilePosition.x = m_origin.x;
        tilePosition.y += m_chunksHeightPixels;
        i = m_firstTileCoordinates.x;
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
bool MapGUI::loadTiles(const json& layer)
{
    PROFILE_FUNCTION();
    namespace mapFile = config::structure::mapFile;
    if (!layer.contains(mapFile::KEY_TILE_DATA))
        return false;
    if (!layer[mapFile::KEY_TILE_DATA].is_array())
        return false;

    unsigned int i = 0;
    unsigned int j = 0;
    sf::Vector2f currentDrawingPosition;
    std::map<unsigned int, std::map<unsigned int, unsigned int>> map;
    for (auto& id : layer[mapFile::KEY_TILE_DATA])
    {
        map[j][i] = id;

        j++;
        if (j >= m_height)
        {
            j = 0;
            i++;
        }
    }
    for (unsigned int bigI = 0; bigI < std::ceil(m_nbChunksWidth); bigI++)
    {
        for (unsigned int bigJ = 0; bigJ < std::ceil(m_nbChunksHeight); bigJ++)
        {
            auto& chunkTexture = m_chunksTextures.emplace_back(
                                     std::make_shared<sf::RenderTexture>());
            unsigned int currentWidth = std::min(CHUNK_WIDTH,
                                                 (m_width - bigI * CHUNK_WIDTH));
            unsigned int currentHeight = std::min(CHUNK_HEIGHT,
                                                  (m_height - bigJ * CHUNK_HEIGHT));
            chunkTexture->create(currentWidth * m_tileWidth, currentHeight * m_tileHeight);

            for (i = 0; i < currentWidth; i++)
            {
                for (j = 0; j < currentHeight; j++)
                {
                    sf::Sprite sprite(m_tiles.at(map[bigI * CHUNK_WIDTH + i][bigJ * CHUNK_HEIGHT +
                                                 j]));
                    sprite.setPosition(i * m_tileWidth, j * m_tileHeight);
                    chunkTexture->draw(sprite);
                }
            }
            sf::Sprite sprite(chunkTexture->getTexture());
            sprite.setOrigin(0, sprite.getGlobalBounds().height);
            sprite.setScale(1, -1);
            m_chunks[bigJ][bigI] = std::move(sprite);
        }
    }

    return true;
}

/**
 * @brief Load the tilesets from the json layer
 *
 * We use Tiled to generate the json : https://www.mapeditor.org/
 * @param json Json object to use
 * @param mapDirPath Directory where are stored the map elements
 * @return Return true if all went well
 */
bool MapGUI::loadTilesets(const std::string& mapDirPath, const json& json)
{
    PROFILE_FUNCTION();
    namespace mapFile = config::structure::mapFile;
    if (!json.contains(mapFile::KEY_WIDTH)
            || !json[mapFile::KEY_WIDTH].is_number_integer())
        return false;
    m_width = json[mapFile::KEY_WIDTH].get<unsigned int>();
    m_nbChunksWidth = m_width / CHUNK_WIDTH;
    if (!json.contains(mapFile::KEY_HEIGHT)
            || !json[mapFile::KEY_HEIGHT].is_number_integer())
        return false;
    m_height = json[mapFile::KEY_HEIGHT].get<unsigned int>();
    m_nbChunksHeight = m_height / CHUNK_HEIGHT;

    if (!json.contains(mapFile::KEY_TILE_WIDTH)
            || !json[mapFile::KEY_TILE_WIDTH].is_number_integer())
        return false;
    m_tileWidth = json[mapFile::KEY_TILE_WIDTH].get<unsigned int>();
    m_chunksWidthPixels = CHUNK_WIDTH * m_tileWidth;
    if (!json.contains(mapFile::KEY_TILE_HEIGHT)
            || !json[mapFile::KEY_TILE_HEIGHT].is_number_integer())
        return false;
    m_tileHeight = json[mapFile::KEY_TILE_HEIGHT].get<unsigned int>();
    m_chunksHeightPixels = CHUNK_HEIGHT * m_tileHeight;

    if (!json.contains(mapFile::KEY_TILESETS)
            || !json[mapFile::KEY_TILESETS].is_array())
        return false;
    auto& tilesets = json[mapFile::KEY_TILESETS];

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
    PROFILE_FUNCTION();
    if (m_centerOfView.x() < 0)
        m_centerOfView.x() = 0;
    if (m_centerOfView.y() < 0)
        m_centerOfView.y() = 0;

    if (m_centerOfView.x() >= (m_width - 1)*m_tileWidth)
        m_centerOfView.x() = (m_width - 1) * m_tileWidth;
    if (m_centerOfView.y() >= (m_height - 1)*m_tileHeight)
        m_centerOfView.y() = (m_height - 1) * m_tileHeight;
}

/**
 * @brief Load the tileset from the json tileset
 *
 * We use Tiled to generate the json : https://www.mapeditor.org/
 * @param tileset Tileset to use
 * @return Return true if all went well
 */
bool MapGUI::loadTileset(const std::string& mapDirPath, const json& tileset)
{
    PROFILE_FUNCTION();
    VLOG(verbosityLevel::FUNCTION_CALL) << "loadTileset";
    namespace mapFile = config::structure::mapFile;
    if (!tileset.is_object())
        return false;
    if (!tileset.contains(mapFile::KEY_TILESET_FIRST_ID)
            || !tileset[mapFile::KEY_TILESET_FIRST_ID].is_number_integer())
        return false;
    unsigned int id = tileset[mapFile::KEY_TILESET_FIRST_ID].get<unsigned int>();

    if (!tileset.contains(mapFile::KEY_TILESET_SOURCE)
            || !tileset[mapFile::KEY_TILESET_SOURCE].is_string())
        return false;

    namespace tilesetFile = config::structure::tilesetFile;
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLError err = doc.LoadFile(std::string(mapDirPath + '/' +
                                          tileset[mapFile::KEY_TILESET_SOURCE].get<std::string>()).c_str());
    if (err)
    {
        LOG(ERROR) << "Error during loading file " <<
                   tileset[mapFile::KEY_TILESET_SOURCE].get<std::string>().c_str();
    }

    auto xmlTileset = doc.FirstChildElement(tilesetFile::ELEMENT_TILESET);
    if (!xmlTileset)
    {
        LOG(ERROR) << "No " << tilesetFile::ELEMENT_TILESET << " in the Tileset " <<
                   mapDirPath << '/' << tileset[mapFile::KEY_TILESET_SOURCE].get<std::string>();
        return false;
    }
    if (!xmlTileset->FindAttribute(tilesetFile::PROPERTY_TILE_WIDTH))
    {
        LOG(ERROR) << "No " << tilesetFile::PROPERTY_TILE_WIDTH << " in the Tileset " <<
                   mapDirPath << '/' << tileset[mapFile::KEY_TILESET_SOURCE].get<std::string>();
        return false;
    }
    unsigned int width = xmlTileset->FindAttribute(
                             tilesetFile::PROPERTY_TILE_WIDTH)->IntValue();

    if (!xmlTileset->FindAttribute(tilesetFile::PROPERTY_TILE_HEIGHT))
    {
        LOG(ERROR) << "No " << tilesetFile::PROPERTY_TILE_HEIGHT << " in the Tileset "
                   << mapDirPath << '/' << tileset[mapFile::KEY_TILESET_SOURCE].get<std::string>();
        return false;
    }
    unsigned int height = xmlTileset->FindAttribute(
                              tilesetFile::PROPERTY_TILE_HEIGHT)->IntValue();

    if (!xmlTileset->FindAttribute(tilesetFile::PROPERTY_TILE_COUNT))
    {
        LOG(ERROR) << "No " << tilesetFile::PROPERTY_TILE_COUNT << " in the Tileset " <<
                   mapDirPath << '/' << tileset[mapFile::KEY_TILESET_SOURCE].get<std::string>();
        return false;
    }
    unsigned int tilecount = xmlTileset->FindAttribute(
                                 tilesetFile::PROPERTY_TILE_COUNT)->IntValue();

    if (!xmlTileset->FindAttribute(tilesetFile::PROPERTY_TILE_COLUMNS))
    {
        LOG(ERROR) << "No " << tilesetFile::PROPERTY_TILE_COLUMNS << " in the Tileset "
                   << mapDirPath << '/' << tileset[mapFile::KEY_TILESET_SOURCE].get<std::string>();
        return false;
    }
    unsigned int columns = xmlTileset->FindAttribute(
                               tilesetFile::PROPERTY_TILE_COLUMNS)->IntValue();

    auto xmlImage = xmlTileset->FirstChildElement(tilesetFile::ELEMENT_IMAGE);
    std::string imageFilename = xmlImage->FindAttribute(
                                    tilesetFile::PROPERTY_IMAGE_SOURCE)->Value();
    imageFilename = mapDirPath + '/' + imageFilename;

    m_textures.emplace_back(std::make_shared<sf::Texture>());
    if (!m_textures.back()->loadFromFile(imageFilename))
        LOG(WARNING) << "Texture from " << imageFilename << " not loaded";

    for (unsigned int line = 0; ; line++)
    {
        for (unsigned int col = 0; col < columns; col++)
        {
            sf::Sprite tile(*m_textures.back(), sf::IntRect(col * width, line * height,
                            width, height));
            m_tiles.insert(std::pair<unsigned int, sf::Sprite>((const unsigned int)id,
                           tile));

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

} // namespace gui

} // namespace map

