#include "Logger.hpp"

/**
 * @brief Get the singleton instance
 */
Logger& Logger::getInstance()
{
    static Logger instance;
    return instance;
}

/**
 * @brief Override of the send method from google::LogSink. Add the message to the buffer
 * @param severity
 * @param full_filename
 * @param base_filename
 * @param line
 * @param tm_time
 * @param message
 * @param message_len
 */
void Logger::send(google::LogSeverity severity, const char* full_filename,
                  const char* base_filename, int line, const tm* tm_time, const char* message,
                  size_t message_len)
{
    m_logs.sputn((std::stringbuf::char_type*)message, message_len);
    m_logs.sputc('\n');
    m_newLogs = true;
}

/**
 * @brief Get all the logs
 * @param size
 * @return
 */
const std::string Logger::getLog(size_t size)
{
    getInstance().m_newLogs = false;
    std::string ret = getInstance().m_logs.str();
    return ret;
}

/**
 * @brief m_newLogs getter
 */
bool Logger::newLogs()
{
    return getInstance().m_newLogs;
}

/**
 * @brief Default Constructor
 */
Logger::Logger()
{

}
