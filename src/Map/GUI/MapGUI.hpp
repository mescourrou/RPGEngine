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

CREATE_EXCEPTION_CLASS(MapGUI)

/**
 * @brief GUI class related to Map
 */
class MapGUI : public BaseGUIObject
{
    DECLARE_BASEOBJECT(MapGUI)
  public:
    MapGUI(std::weak_ptr<map::Map> map);
    /// @brief Default destructor
    ~MapGUI() override = default;

    void move(double offsetX, double offsetY);
    void setCenterOfView(const Position& centralPosition);
    bool load(const std::string& mapDirPath) override;

    void prepare(const sf::Vector2f& targetSize) override;
    void forcePrepare(const sf::Vector2f& targetSize);

    /**
     * @brief The on-screen position of the map does not move
     */
    void setOnScreenPosition(const sf::Vector2f&) override {}
    sf::Vector2f positionOnScreenFrom(const map::Position& position);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
  protected:
    bool loadTiles(const json& layer);
    bool loadTilesets(const std::string& mapDirPath, const json& json);

  private:
    void saturateCenterOfView();
    bool loadTileset(const std::string& mapDirPath, const json& tileset);

    std::map<unsigned int, sf::Sprite>
    m_tiles;         ///< List of tiles and their id
    mutable std::map<unsigned int, std::map<unsigned int, sf::Sprite>>
            m_chunks; ///< Id of tiles according to the position

    static constexpr unsigned int CHUNK_WIDTH = 50;
    static constexpr unsigned int CHUNK_HEIGHT = 50;

    Vector<2> m_centerOfView;               ///< Center of the view
    sf::Vector2f
    m_topLeftPosition;         ///< Position on the map of the top left corner of the screen (in pixels)
    sf::Vector2f
    m_origin;                  ///< Position on the screen of the top left displayed tile
    sf::Vector2i
    m_firstTileCoordinates;    ///< Coordinates of the tile of the top left corner on the map (in pixel)
    bool m_mapMoved =
        true;                 ///< The location changed so we need to compute again some parameters

    std::vector<std::shared_ptr<sf::Texture>>
                                           m_textures;   ///< List of textures, freed in the destructor
    std::vector<std::shared_ptr<sf::RenderTexture>>
            m_chunksTextures;   ///< List of textures, freed in the destructor

    unsigned int m_height;                  ///< Height of the map (in tiles)
    unsigned int m_width;                   ///< Width of the map (in tiles)
    unsigned int m_nbChunksWidth;
    unsigned int m_nbChunksHeight;

    unsigned int m_chunksWidthPixels;
    unsigned int m_chunksHeightPixels;

    unsigned int m_tileHeight;              ///< Height of a tile (in pixel)
    unsigned int m_tileWidth;               ///< Width of a tile (in pixel)

    float m_zoom = 1;                       ///< Zoom @todo: Use it

    std::weak_ptr<map::Map> m_map;          ///< Pointer on the backend map

};

} // namespace gui

} // namespace map
