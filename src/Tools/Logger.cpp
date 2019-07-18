#include "Logger.hpp"

Logger &Logger::getInstance()
{
    static Logger instance;
    return instance;
}


void Logger::send(google::LogSeverity severity, const char *full_filename,
                  const char *base_filename, int line, const tm *tm_time, const char *message, size_t message_len)
{
    m_logs.sputn((std::stringbuf::char_type*)message, message_len);
    m_logs.sputc('\n');
    m_newLogs = true;
}

const std::string Logger::getLog()
{
    getInstance().m_newLogs = false;
    return getInstance().m_logs.str();
}

bool Logger::newLogs()
{
    return getInstance().m_newLogs;
}


Logger::Logger()
{

}
