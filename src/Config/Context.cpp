#include "Context.hpp"
// I/O
#include <memory>

#include <filesystem>

// Project
#include <Config.hpp>
#include <VerbosityLevels.hpp>

// External libs
#include <glog/logging.h>
/**
 * @brief Initialize a context from argc and argv
 * @param argc Number of arguments in argv
 * @param argv Main function arguments
 */
config::Context::Context(int argc, char **argv)
{
    VLOG(verbosityLevel::OBJECT_CREATION) << "Creating " << className() << " => " << this;
    std::filesystem::path exec = std::string(argv[0]);
    if (argv)
        m_runtimeDirectory =  exec.parent_path();
    if (argc > 1)
    {
        for (unsigned int i = 1; i < argc; i++)
        {
            m_programArguments.push_back(argv[i]);
        }
    }
    try {
        m_config.reset(new Config(std::string().append(m_runtimeDirectory).append("/")
                                  .append(m_kConfigPath).append("/")
                                  .append(m_kGlobalConfigFilename)));
    } catch (const Config::ConfigException& e) {
        LOG(WARNING) << "Error during loading the configuration : " << e.what();
        m_config.reset();
    }

}

