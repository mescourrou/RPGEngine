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

class Context : public BaseObject
{
#ifdef RPG_BUILD_TEST
    friend class config::ContextTest;
    FRIEND_TEST(ContextTest, Initialization);
#endif
public:
    Context() = delete;
    ~Context() override = default;
    static std::shared_ptr<const Context> get();
    static void initialize(int argc, char **argv);
    static void kill();

    std::string className() const noexcept override { return "Context"; }

    std::string currentDirectory() const { return m_currentDirectory; }
    std::shared_ptr<Config> config() const { return  m_config; }
    const std::string kConfigPath() const { return m_kConfigPath; }

protected:
    Context(int argc, char **argv);

private:
    static std::shared_ptr<Context> m_instance;
    int m_argc;
    char** m_argv;

    // Constants
    const std::string m_kConfigPath = "config";
    const std::string m_kGlobalConfigFilename = "global.ini";

    // Global variables
    std::string m_currentDirectory = "";
    std::shared_ptr<Config> m_config;
};

} // namespace config

