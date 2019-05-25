#pragma once

// Project
#include "general_config.hpp"
#include <BaseObject.hpp>

#ifdef RPG_BUILD_TEST
#include <gtest/gtest.h>
#endif

namespace config
{
class Config;

#ifdef RPG_BUILD_TEST
class ContextTest;
#endif

/**
 * @brief Group global variables.
 */
class Context : public BaseObject
{
    DECLARE_BASEOBJECT(Context)
#ifdef RPG_BUILD_TEST
    friend class config::ContextTest;
    FRIEND_TEST(ContextTest, Initialization);
#endif
public:
    Context() = delete;
    Context(int argc, char **argv);
    ~Context() override = default;

    /// @brief Return the path to the runtime directory
    std::string runtimeDirectory() const { return m_runtimeDirectory; }
    /// @brief Return the config
    std::shared_ptr<Config> config() const { return  m_config; }
    /// @brief Access the config directory (from runtime directory)
    const std::string kConfigPath() const { return m_kConfigPath; }
    const std::string kMapPath() const { return m_kRessourcesDirPath + '/' + m_kMapDirPath; }
    /// @brief Program arguments
    const std::vector<std::string>& kProgramArguments() const { return m_programArguments; }
    /// @brief Get the location of the current running game
    std::string& gameLocation() { return m_gameLocation; }

private:
    std::vector<std::string> m_programArguments; ///< List of program arguments (from argv)

    // Constants
    const std::string m_kConfigPath = "config"; ///< Directory where are the config files
    const std::string m_kGlobalConfigFilename = "global.ini"; ///< Main config filename

    const std::string m_kRessourcesDirPath = "ressources";
    const std::string m_kMapDirPath = "map";

    // Global variables
    std::string m_runtimeDirectory = ""; ///< Path to the runtime directory
    std::shared_ptr<Config> m_config; ///< Configuration
    std::string m_gameLocation = "";
};

} // namespace config

