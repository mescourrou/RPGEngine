#include "Context.hpp"
/**
 * @brief Protected constructor
 * @param argc Number of arguments in argv
 * @param argv Main function arguments
 */
config::Context::Context(int argc, char **argv) : m_argc(argc), m_argv(argv)
{
    if (argv)
        m_runtimeDirectory = std::string(argv[0]);
    m_config.reset(new Config(std::string().append(m_runtimeDirectory).append("/")
                              .append(m_kConfigPath).append("/")
                              .append(m_kGlobalConfigFilename)));
}

