#include "Context.hpp"

std::shared_ptr<config::Context> config::Context::m_instance;

std::shared_ptr<const config::Context> config::Context::get()
{
    if (!m_instance)
        LOG(ERROR) << "Context not initialized";
    return m_instance;
}

void config::Context::initialize(int argc, char **argv)
{
    m_instance.reset(new Context(argc, argv));
}

void config::Context::kill()
{
    m_instance.reset();
}

config::Context::Context(int argc, char **argv) : m_argc(argc), m_argv(argv), m_currentDirectory(argv[0])
{
    m_config.reset(new Config(std::string().append(m_currentDirectory).append("/")
                              .append(m_kConfigPath).append("/")
                              .append(m_kGlobalConfigFilename)));
}

