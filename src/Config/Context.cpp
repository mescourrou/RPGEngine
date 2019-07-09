#include "Context.hpp"
// Stl
#include <memory>
#ifdef BUILD_USE_FILESYSTEM
#include <filesystem>
#endif

// Project
#include <Config.hpp>

/**
 * @brief Initialize a context from argc and argv
 * @param argc Number of arguments in argv
 * @param argv Main function arguments
 */
config::Context::Context(int argc, char **argv)
{
    VLOG(verbosityLevel::OBJECT_CREATION) << "Creating " << className() << " => " << this;
    if (!argv)
    {
        LOG(ERROR) << "Error : argv pointer is null";
        return;
    }
    if (argc >= 1)
    {
#ifdef BUILD_USE_FILESYSTEM
        m_runtimeDirectory = std::filesystem::path(argv[0]).parent_path();
#else
        m_runtimeDirectory = argv[0];
        auto lastSeparator = m_runtimeDirectory.find_last_of('/');
        m_runtimeDirectory = m_runtimeDirectory.substr(0, lastSeparator);
        VLOG(verbosityLevel::VERIFICATION_LOG) << "Runtime directory = " << m_runtimeDirectory;
#endif
    }
    if (argc > 1)
    {
        for (int i = 1; i < argc; i++)
        {
            m_programArguments.push_back(argv[i]);
        }
    }
    try {
        m_config.reset(new Config(std::string().append(m_runtimeDirectory).append("/")
                                  .append(m_kConfigPath).append("/")
                                  .append(m_kGlobalConfigFilename)));
    } catch (const ConfigException& e) {
        LOG(WARNING) << "Error during loading the configuration : " << e.what();
        m_config.reset();
    }

}

