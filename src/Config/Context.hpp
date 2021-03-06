#pragma once

// Stl
#include <memory>
#include <chrono>

// Project
#include "general_config.hpp"
#include <BaseObject.hpp>
#include <VerbosityLevels.hpp>

// External libs
#include <glog/logging.h>

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
    Context(int argc, char** argv);
    ~Context() override = default;

    /// @brief Return the path to the runtime directory
    virtual std::string runtimeDirectory() const
    {
        return m_runtimeDirectory;
    }
    /// @brief Return the config
    virtual std::shared_ptr<Config> config() const
    {
        return  m_config;
    }
    /// @brief Access the config directory (from runtime directory)
    virtual const std::string kConfigPath() const
    {
        return m_kConfigPath;
    }
    /// @brief Get the name of the global config file
    virtual const std::string kGlobalConfigFilename() const
    {
        return m_kGlobalConfigFilename;
    }
    /// @brief Get the full path to the map directory
    virtual const std::string kMapPath() const
    {
        return std::string(m_gameLocation).append("/")
               .append(m_kRessourcesDirPath).append("/").append(m_kMapDirPath);
    }
    /// @brief Get the full path to the character directory
    virtual const std::string kCharacterPath() const
    {
        return std::string(m_gameLocation).append("/")
               .append(m_kRessourcesDirPath).append("/").append(m_kCharacterDirPath);
    }

    /// @brief Program arguments
    virtual const std::vector<std::string>& kProgramArguments() const
    {
        VLOG(verbosityLevel::FUNCTION_CALL) << "kProgramArguments";
        return m_programArguments;
    }
    /// @brief Get the location of the current running game
    virtual std::string& gameLocation()
    {
        return m_gameLocation;
    }

    std::time_t
    framePeriod;                                    ///< Period since the last frame

  protected:
    Context() = default; // For mocking
  private:
    std::vector<std::string>
    m_programArguments;                ///< List of program arguments (from argv)

    // Constants
    const std::string m_kConfigPath =
        "config";                 ///< Directory where are the config files
    const std::string m_kGlobalConfigFilename =
        "global.ini";   ///< Main config filename

    const std::string m_kRessourcesDirPath =
        "ressources";      ///< Path of the ressource directory
    const std::string m_kMapDirPath =
        "map";                    ///< Name of the map directory
    const std::string m_kCharacterDirPath =
        "character";        ///< Name of the character directory

    // Global variables
    std::string m_runtimeDirectory =
        "";                        ///< Path to the runtime directory
    std::shared_ptr<Config> m_config;                           ///< Configuration
    std::string m_gameLocation =
        "";                            ///< Path the directory containing the game
};

} // namespace config

