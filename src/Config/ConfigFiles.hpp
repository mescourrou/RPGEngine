#pragma once

namespace config {
namespace structure {

// global file
namespace globalFile {
static constexpr char MAIN_CONFIGURATION_FILE[] = "main.ini";
namespace ressources {
constexpr char SECTION[] = "Ressources";
constexpr char DATABASE[] = "database";
} // namespace ressources

namespace preferences {
constexpr char SECTION[] = "Preferences";
constexpr char FULLSCREEN[] = "fullscreen";
constexpr char RESOLUTION[] = "resolution";


} // namespace preferences

} // namespace globalFile

} // namespace structure
} // namespace config
