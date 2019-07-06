#pragma once

#include <memory>

#include <SFML/Graphics/Shape.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include <BaseException.hpp>
#include <Vector.hpp>
#include <Map.hpp>
#include <Context.hpp>
#include <memory>

namespace map {

namespace GUI {

CREATE_EXCEPTION_CLASS(MapGUI)

class MapGUI : public Map, public sf::Transformable, public sf::Drawable
{
    DECLARE_BASEOBJECT(MapGUI)
public:
    MapGUI(std::shared_ptr<config::Context> context, const std::string& name);
    ~MapGUI() override final;

    void move(double offsetX, double offsetY);

protected:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    bool doLoadTiles(const json &layer) override;
    bool doLoadTilesets(const json& json) override;

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
    bool loadTileset(const json& tileset);

    std::map<unsigned int, sf::Sprite> m_tiles;
    std::map<unsigned int, std::map<unsigned int, unsigned int>> m_idMap;

    Vector<2> m_centerOfView;

    std::vector<sf::Texture*> m_textures;

    unsigned int m_height;
    unsigned int m_width;

    unsigned int m_tileHeight;
    unsigned int m_tileWidth;

    float m_zoom = 1;

};

} // namespace GUI

} // namespace map
