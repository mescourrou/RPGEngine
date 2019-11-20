#pragma once

#include <iostream>
#include <fstream>
#include <algorithm>
#include <mutex>

#include <general_config.hpp>

#ifdef PROFILING
#define BEGIN_SESSION(name) Instrumentor::get().beginSession(name)
#define END_SESSION() Instrumentor::get().endSession()
#else
#define BEGIN_SESSION(name)
#define END_SESSION()
#endif

/**
 * @brief Profile result description
 */
struct ProfileResult
{
    std::string name; ///< Name of the profile
    long long start; ///< Start timepoint
    long long end; ///< End timepoint
    uint32_t threadID; ///< ID of the profile thread
};

/**
 * @brief Session description
 */
struct InstrumentationSession
{
    std::string name; ///< Name of the session
};

/**
 * @brief Singleton managing the profiling sessions and writings
 */
class Instrumentor
{
public:

    void beginSession(const std::string& name);

    void beginSession(const std::string& name, const std::string& filepath);

    void endSession();

    void writeProfile(const ProfileResult& result);

    void writeHeader();

    void writeFooter();

    static Instrumentor& get()
    {
        static Instrumentor instance;
        return instance;
    }

private:
    Instrumentor();
    ~Instrumentor();
    InstrumentationSession* m_currentSession;
    std::ofstream m_outputStream;
    int m_profileCount;
    std::mutex m_mutex;
};

