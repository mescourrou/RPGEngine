#pragma once

namespace config {

/**
 * @namespace structure
 * @brief Define the keywords of the .ini configuration files
 */
namespace structure {

/**
 * @namespace globalFile
 * @brief Keyword of the main file
 */
namespace globalFile {
/// Main file name
static constexpr char FILE_NAME[] = "main.ini";
/**
 * @namespace ressources
 * @brief Ressource section
 */
namespace ressources {
/// Section name
constexpr char SECTION[] = "Ressources";
/// Keyword for the database location
constexpr char DATABASE[] = "database";
} // namespace ressources

/**
 * @namespace preferences
 * @brief User reference section for saving
 */
namespace preferences {
/// Section name
constexpr char SECTION[] = "Preferences";
/// Fullscreen : true/false
constexpr char FULLSCREEN[] = "fullscreen";
/// Resolution : 1920x1080 par exemple
constexpr char RESOLUTION[] = "resolution";


} // namespace preferences

} // namespace globalFile


namespace gameListFile {
/// Key to find the directory in .ini file
constexpr char DIRECTORY_KEY[] = "directory";
} // namespace gameListFile

} // namespace structure
} // namespace config
