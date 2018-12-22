#include "Context.hpp"
// I/O
#include <memory>

// Project
#include <Config.hpp>
/**
 * @brief Initialize a context from argc and argv
 * @param argc Number of arguments in argv
 * @param argv Main function arguments
 */
config::Context::Context(int argc, char **argv)
{
    if (argv)
        m_runtimeDirectory = std::string(argv[0]);
    if (argc > 1)
    {
        for (unsigned int i = 1; i < argc; i++)
        {
            m_programArguments.push_back(argv[i]);
        }
    }

    m_config.reset(new Config(std::string().append(m_runtimeDirectory).append("/")
                              .append(m_kConfigPath).append("/")
                              .append(m_kGlobalConfigFilename)));
}

