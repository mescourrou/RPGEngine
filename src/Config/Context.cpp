#include "Context.hpp"

std::shared_ptr<config::Context> config::Context::m_instance;

/**
 * @brief Get the singleton instance
 * @warning The context must be initialized before, otherwise, empty shared_ptr is returned
 * @return Singleton instance
 */
std::shared_ptr<const config::Context> config::Context::get()
{
    if (!m_instance)
        LOG(ERROR) << "Context not initialized";
    return m_instance;
}

/**
 * @brief Initialize the context. If the context is already initialized, the context is modified
 *
 * Argc and argv are the parameters of the main function
 * @param argc Size of argv
 * @param argv Program parameters
 */
void config::Context::initialize(int argc, char **argv)
{
    m_instance.reset(new Context(argc, argv));
}

/**
 * @brief Delete the Context instance
 */
void config::Context::kill()
{
    m_instance.reset();
}

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

