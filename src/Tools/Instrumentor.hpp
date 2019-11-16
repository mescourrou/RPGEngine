#pragma once

#include <iostream>
#include <fstream>
#include <algorithm>

#include <general_config.hpp>

#ifdef PROFILING
#define BEGIN_SESSION(name) Instrumentor::get().beginSession(name)
#define END_SESSION() Instrumentor::get().endSession()
#else
#define BEGIN_SESSION(name)
#define END_SESSION()
#endif

struct ProfileResult
{
    std::string name;
    long long start, end;
    uint32_t threadID;
};

struct InstrumentationSession
{
    std::string name;
};

class Instrumentor
{
private:
    InstrumentationSession* m_currentSession;
    std::ofstream m_outputStream;
    int m_profileCount;
public:
    Instrumentor();

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
};

