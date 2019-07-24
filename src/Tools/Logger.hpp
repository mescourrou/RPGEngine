#pragma once

#include <glog/logging.h>
#include <mutex>

class Logger : public google::LogSink
{
public:
    static Logger& getInstance();

    void send(google::LogSeverity severity,
              const char *full_filename,
              const char *base_filename,
              int line, const tm *tm_time,
              const char *message, size_t message_len) override;
    static const std::string getLog(size_t size = 0);
    static bool newLogs();
private:
    Logger();
    std::stringbuf m_logs;
    size_t m_size;
    bool m_newLogs = false;
    std::mutex m_mutex;
};
