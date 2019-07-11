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
    DECLARE_BASEOBJECT(MapGUI)
public:
    MapGUI(std::weak_ptr<map::Map> map);
    /// @brief Default destructor
    ~MapGUI() override = default;

    void move(double offsetX, double offsetY);
    void setCenterOfView(const Position& centralPosition);
    bool load(const std::string& mapDirPath) override;

    void prepare(const sf::Vector2u& targetSize) override;

    void setOnScreenPosition(const sf::Vector2f&) override {}
    sf::Vector2f positionOnScreenFrom(const map::Position& position);

    void setTarget(std::weak_ptr<sf::RenderTarget> target, const sf::RenderStates& states = sf::RenderStates::Default);

protected:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    bool loadTiles(const json &layer);
    bool loadTilesets(const std::string &mapDirPath, const json& json);

private:
    void saturateCenterOfView();
    bool loadTileset(const std::string& mapDirPath, const json& tileset);

    std::map<unsigned int, sf::Sprite> m_tiles;         ///< List of tiles and their id
    std::map<unsigned int, std::map<unsigned int, unsigned int>> m_idMap; ///< Id of tiles according to the position

    Vector<2> m_centerOfView;               ///< Center of the view
    sf::Vector2f m_topLeftPosition;         ///< Position on the map of the top left corner of the screen (in pixels)
    sf::Vector2f m_origin;
    sf::Vector2i m_firstTileCoordinates;
    bool m_somethingChanged = true;

    std::vector<std::unique_ptr<sf::Texture>> m_textures;   ///< List of textures, freed in the destructor

    unsigned int m_height;                  ///< Height of the map (in tiles)
    unsigned int m_width;                   ///< Width of the map (in tiles)

    unsigned int m_tileHeight;              ///< Height of a tile (in pixel)
    unsigned int m_tileWidth;               ///< Width of a tile (in pixel)

    float m_zoom = 1;                       ///< Zoom @todo: Use it

    std::weak_ptr<sf::RenderTarget> m_target;
    sf::RenderStates m_states;

    std::weak_ptr<map::Map> m_map;

};

} // namespace GUI

} // namespace map
