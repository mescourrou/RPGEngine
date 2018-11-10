#pragma once

// I/O
#include <memory>

// Project
#include "general_config.hpp"
#include <BaseObject.hpp>
#include <Config.hpp>

#ifdef RPG_BUILD_TEST
#include <gtest/gtest.h>
#endif

namespace config
{

#ifdef RPG_BUILD_TEST
class ContextTest;
#endif

/**
 * @brief Singleton keeping the global variables
 */
class Context : public BaseObject
{
#ifdef RPG_BUILD_TEST
    friend class config::ContextTest;
    FRIEND_TEST(ContextTest, Initialization);
#endif
public:
    Context() = delete;
    Context(int argc, char **argv);
    ~Context() override = default;

    std::string className() const noexcept override { return "Context"; }

    /// @brief Return the path to the runtime directory
    std::string runtimeDirectory() const { return m_runtimeDirectory; }
    /// @brief Return the config
    std::shared_ptr<Config> config() const { return  m_config; }
    /// @brief Access the config directory (from runtime directory)
    const std::string kConfigPath() const { return m_kConfigPath; }

private:
    int m_argc; ///< Number of arguments in m_argv
    char** m_argv; ///< Arguments of the program

    // Constants
    const std::string m_kConfigPath = "config"; ///< Directory where are the config files
    const std::string m_kGlobalConfigFilename = "global.ini"; ///< Main config filename

    // Global variables
    std::string m_runtimeDirectory = ""; ///< Path to the runtime directory
    std::shared_ptr<Config> m_config; ///< Configuration
};

} // namespace config

