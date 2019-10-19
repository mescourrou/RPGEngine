#pragma once

namespace config
{

/**
 * @namespace structure
 * @brief Define the keywords of the configuration files (ini, json, xml, ...)
 */
namespace structure
{

/**
 * @namespace globalFile
 * @brief Keyword of the main file
 */
namespace globalFile
{
/// Main file name
constexpr char FILE_NAME[] = "main.ini";
/**
 * @namespace ressources
 * @brief Ressource section
 */
namespace ressources
{
/// Section name
constexpr char SECTION[] = "Ressources";
/// Keyword for the database location
constexpr char DATABASE[] = "database";
} // namespace ressources

/**
 * @namespace preferences
 * @brief User reference section for saving
 */
namespace preferences
{
/// Section name
constexpr char SECTION[] = "Preferences";
/// Fullscreen : true/false
constexpr char FULLSCREEN[] = "fullscreen";
/// Resolution : 1920x1080 par exemple
constexpr char RESOLUTION[] = "resolution";


} // namespace preferences

} // namespace globalFile


namespace gameListFile
{
/// Key to find the directory in .ini file
constexpr char DIRECTORY_KEY[] = "directory";
} // namespace gameListFile

namespace mapFile
{
constexpr char KEY_LAYERS[] = "layers";
constexpr char KEY_LAYER_NAME[] = "name";
constexpr char KEY_LAYER_TYPE[] = "type";
constexpr char TYPE_DATA_LAYER[] = "tilelayer";
constexpr char NAME_COLLISIONS_LAYER[] = "collisions";
constexpr char NAME_TELEPORTS_LAYER[] = "teleports";
constexpr char KEY_OBJECTS[] = "objects";
constexpr char KEY_VISIBLE[] = "visible";
constexpr char KEY_X[] = "x";
constexpr char KEY_Y[] = "y";
constexpr char KEY_HEIGHT[] = "height";
constexpr char KEY_WIDTH[] = "width";
constexpr char KEY_POLYGON[] = "polygon";

// Tile section
constexpr char KEY_TILE_DATA[] = "data";
constexpr char KEY_TILESETS[] = "tilesets";
constexpr char KEY_TILE_HEIGHT[] = "tileheight";
constexpr char KEY_TILE_WIDTH[] = "tilewidth";
constexpr char KEY_TILESET_FIRST_ID[] = "firstgid";
constexpr char KEY_TILESET_SOURCE[] = "source";
} // namespace mapFiles

namespace tilesetFile
{
constexpr char ELEMENT_TILESET[] = "tileset";
constexpr char ELEMENT_IMAGE[] = "image";
constexpr char PROPERTY_TILE_WIDTH[] = "tilewidth";
constexpr char PROPERTY_TILE_HEIGHT[] = "tileheight";
constexpr char PROPERTY_TILE_COUNT[] = "tilecount";
constexpr char PROPERTY_TILE_COLUMNS[] = "columns";
constexpr char PROPERTY_IMAGE_SOURCE[] = "source";
constexpr char PROPERTY_IMAGE_WIDTH[] = "width";
constexpr char PROPERTY_IMAGE_HEIGHT[] = "height";
} // namespace tilesetFile

namespace characterFile
{
constexpr char SPRITE_SETS[] = "spriteSets";
constexpr char FIRST_ID[] = "firstId";
constexpr char SET_FILE[] = "setFile";
constexpr char HEIGHT[] = "height";
constexpr char WIDTH[] = "width";
constexpr char SPRITE_HEIGHT[] = "spriteHeight";
constexpr char SPRITE_WIDTH[] = "spriteWidth";
constexpr char HORIZONTAL_SHIFT[] = "horizontalShift";
constexpr char VERTICAL_SHIFT[] = "verticalShift";
constexpr char BACKGROUND[] = "background";
constexpr char ORIGIN_X[] = "originX";
constexpr char ORIGIN_Y[] = "originY";
constexpr char SPRITE_PERIOD[] = "spritePeriod";
constexpr char ACTIONS[] = "actions";

} // namespace characterFile

} // namespace structure
} // namespace config
