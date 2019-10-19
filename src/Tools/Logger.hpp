#pragma once

#include <glog/logging.h>
#include <mutex>

/**
 * @brief Google log's sink to store and access logs
 */
class Logger : public google::LogSink
{
  public:
    static Logger& getInstance();

    void send(google::LogSeverity severity,
              const char* full_filename,
              const char* base_filename,
              int line, const tm* tm_time,
              const char* message, size_t message_len) override;
    static const std::string getLog(size_t size = 0);
    static bool newLogs();
  private:
    Logger();

    std::stringbuf m_logs;      ///< Log buffer
    std::mutex m_mutex;         ///< Mutex to access the log buffer
    bool m_newLogs = false;     ///< Is there new logs sing the last "getLog"
};
