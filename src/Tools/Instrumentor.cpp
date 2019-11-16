#include "Instrumentor.hpp"

Instrumentor::Instrumentor()
    : m_currentSession(nullptr), m_profileCount(0)
{
}

void Instrumentor::beginSession(const std::string &name)
{
    beginSession(name, name+"_result.json");
}

void Instrumentor::beginSession(const std::string &name, const std::string &filepath)
{
    m_outputStream.open(filepath);
    writeHeader();
    m_currentSession = new InstrumentationSession{ name };
}

void Instrumentor::endSession()
{
    writeFooter();
    m_outputStream.close();
    delete m_currentSession;
    m_currentSession = nullptr;
    m_profileCount = 0;
}

void Instrumentor::writeProfile(const ProfileResult &result)
{
    if (m_profileCount++ > 0)
        m_outputStream << ",";

    std::string name = result.name;
    std::replace(name.begin(), name.end(), '"', '\'');

    m_outputStream << "{";
    m_outputStream << "\"cat\":\"function\",";
    m_outputStream << "\"dur\":" << (result.end - result.start) << ',';
    m_outputStream << "\"name\":\"" << name << "\",";
    m_outputStream << "\"ph\":\"X\",";
    m_outputStream << "\"pid\":0,";
    m_outputStream << "\"tid\":" << result.threadID << ",";
    m_outputStream << "\"ts\":" << result.start;
    m_outputStream << "}";

    m_outputStream.flush();
}

void Instrumentor::writeHeader()
{
    m_outputStream << "{\"otherData\": {},\"traceEvents\":[";
    m_outputStream.flush();
}

void Instrumentor::writeFooter()
{
    m_outputStream << "]}";
    m_outputStream.flush();
}
