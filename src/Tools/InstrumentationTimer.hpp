#pragma once

#include <chrono>
#include <thread>

#include <Instrumentor.hpp>

#ifdef PROFILING
#define PROFILE_SCOPE(name) InstrumentationTimer timer##__LINE__ (name)
#define PROFILE_FUNCTION() PROFILE_SCOPE(__FUNCSIG__)
#else
#define PROFILE_SCORE(name)
#define PROFILE_FUNCTION()
#endif

class InstrumentationTimer
{
public:
    InstrumentationTimer(const char* name) : m_name(name), m_stopped(false)
    {
        m_start = std::chrono::high_resolution_clock::now();
    }

    ~InstrumentationTimer()
    {
        if (!m_stopped)
            stop();
    }

    void stop() {
        auto endTimepoint = std::chrono::high_resolution_clock::now();
        long long start = std::chrono::time_point_cast<std::chrono::nanoseconds>(m_start).time_since_epoch().count();
        long long end = std::chrono::time_point_cast<std::chrono::nanoseconds>(endTimepoint).time_since_epoch().count();

        uint32_t threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());
        Instrumentor::get().writeProfile({ m_name, start, end, threadID });

        m_stopped = true;
    }


private:
    const char* m_name;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_start;
    bool m_stopped;
};

