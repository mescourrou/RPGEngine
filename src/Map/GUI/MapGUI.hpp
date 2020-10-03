#pragma once

// Stl
#include <memory>

// Project
#include <BaseException.hpp>
#include <BaseGUIObject.hpp>
#include <Vector.hpp>
#include <Context.hpp>
#include <Map.hpp>

// External lib
#include <SFML/Graphics/Shape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTexture.hpp>


// External libs
#include <json.hpp>
using json = nlohmann::json;

namespace map
{

namespace gui
{

CREATE_EXCEPTION_CLASS(MapGUI);

/**
 * @brief GUI class related to Map
 */
class MapGUI : public BaseGUIObject
{
    DECLARE_BASEOBJECT(MapGUI);
  public:
    explicit MapGUI(std::weak_ptr<map::Map> map);
    /// @brief Default destructor
    ~MapGUI() override = default;

    void moveCenterOfView(double offsetX, double offsetY);
    void setCenterOfView(const Position& centralPosition);
    bool load(const std::string& mapDirPath) override;

    void prepare(const sf::Vector2f& targetSize) override;
    void forcePrepare(const sf::Vector2f& targetSize);

    void setOnScreenPosition(const sf::Vector2f&) override
    {
        // The on-screen position of the map does not move
    }
    sf::Vector2f positionOnScreenFrom(const map::Position& position);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
  protected:
    bool loadTiles(const json& layer);
    bool loadTilesets(const std::string& mapDirPath, const json& json);

  private:
    void saturateCenterOfView();
    bool loadTileset(const std::string& mapDirPath, const json& tileset);

    /// List of tiles and their id
    std::map<unsigned int, sf::Sprite> m_tiles;
    /// Id of tiles according to the position
    mutable std::map<unsigned int, std::map<unsigned int, sf::Sprite>> m_chunks;

    static constexpr unsigned int CHUNK_WIDTH = 50;
    static constexpr unsigned int CHUNK_HEIGHT = 50;

    /// Center of the view
    Vector<2> m_centerOfView;
    /// Position on the map of the top left corner of the screen (in pixels)
    sf::Vector2f m_topLeftPosition;
    /// Position on the screen of the top left displayed tile
    sf::Vector2f m_origin;
    /// Coordinates of the tile of the top left corner on the map (in pixel)
    sf::Vector2i m_firstTileCoordinates;
    /// The location changed so we need to compute again some parameters
    bool m_mapMoved = true;

    /// List of textures, freed in the destructor
    std::vector<std::shared_ptr<sf::Texture>> m_textures;
    /// List of textures, freed in the destructor
    std::vector<std::shared_ptr<sf::RenderTexture>> m_chunksTextures;

    /// Height of the map (in tiles)
    unsigned int m_height;
    /// Width of the map (in tiles)
    unsigned int m_width;
    unsigned int m_nbChunksWidth;
    unsigned int m_nbChunksHeight;

    unsigned short m_chunksWidthPixels;
    unsigned short m_chunksHeightPixels;

    /// Height of a tile (in pixel)
    unsigned int m_tileHeight;
    /// Width of a tile (in pixel)
    unsigned int m_tileWidth;

    /// Pointer on the backend map
    std::weak_ptr<map::Map> m_map;

};

} // namespace gui

} // namespace map
