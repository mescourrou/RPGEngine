#pragma once

// Stl
#include <memory>

// Project
#include <BaseException.hpp>
#include <BaseGUIObject.hpp>
#include <Vector.hpp>
#include <Context.hpp>

// External lib
#include <SFML/Graphics/Shape.hpp>
#include <SFML/Graphics/Sprite.hpp>


// External libs
#include <json.hpp>
using json = nlohmann::json;

namespace map {

namespace GUI {

CREATE_EXCEPTION_CLASS(MapGUI)

/**
 * @brief GUI part of the Map
 */
class MapGUI : public BaseGUIObject
{
public:
    MapGUI() = default;
    ~MapGUI() override;

    void move(double offsetX, double offsetY);
    void setCenterOfView(const Vector<2>& centerOfView);

    void prepare() override {}

protected:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    bool loadTiles(const json &layer);
    bool loadTilesets(const std::string &mapDirPath, const json& json);

    static inline constexpr char KEY_TILE_DATA[] = "data";
    static inline constexpr char KEY_TILESETS[] = "tilesets";
    static inline constexpr char KEY_HEIGHT[] = "height";
    static inline constexpr char KEY_WIDTH[] = "width";
    static inline constexpr char KEY_TILE_HEIGHT[] = "tileheight";
    static inline constexpr char KEY_TILE_WIDTH[] = "tilewidth";
    static inline constexpr char KEY_TILESET_FIRST_ID[] = "firstgid";
    static inline constexpr char KEY_TILESET_SOURCE[] = "source";
    static inline constexpr char ELEMENT_TILESET[] = "tileset";
    static inline constexpr char ELEMENT_IMAGE[] = "image";
    static inline constexpr char PROPERTY_TILE_WIDTH[] = "tilewidth";
    static inline constexpr char PROPERTY_TILE_HEIGHT[] = "tileheight";
    static inline constexpr char PROPERTY_TILE_COUNT[] = "tilecount";
    static inline constexpr char PROPERTY_TILE_COLUMNS[] = "columns";
    static inline constexpr char PROPERTY_IMAGE_SOURCE[] = "source";
    static inline constexpr char PROPERTY_IMAGE_WIDTH[] = "width";
    static inline constexpr char PROPERTY_IMAGE_HEIGHT[] = "height";
private:
    void saturateCenterOfView();
    bool loadTileset(const std::string& mapDirPath, const json& tileset);

    std::map<unsigned int, sf::Sprite> m_tiles;         ///< List of tiles and their id
    std::map<unsigned int, std::map<unsigned int, unsigned int>> m_idMap; ///< Id of tiles according to the position

    Vector<2> m_centerOfView;               ///< Center of the view

    std::vector<sf::Texture*> m_textures;   ///< List of textures, freed in the destructor

    unsigned int m_height;                  ///< Height of the map (in tiles)
    unsigned int m_width;                   ///< Width of the map (in tiles)

    unsigned int m_tileHeight;              ///< Height of a tile (in pixel)
    unsigned int m_tileWidth;               ///< Width of a tile (in pixel)

    float m_zoom = 1;                       ///< Zoom @todo: Use it

};

} // namespace GUI

} // namespace map
