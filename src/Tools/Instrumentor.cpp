#include "Instrumentor.hpp"

/**
 * @brief Default constructor.
 */
Instrumentor::Instrumentor()
    : m_currentSession(nullptr), m_profileCount(0)
{
}

/**
 * @brief Finish the session if endSession was not called.
 */
Instrumentor::~Instrumentor()
{
    if (m_currentSession)
        endSession();
}

/**
 * @brief Begin a profiling session (single file).
 * @param name Name of the session
 */
void Instrumentor::beginSession(const std::string& name)
{
    beginSession(name, name + "_result.json");
}

/**
 * @brief Begin a profiling session (single file).
 * @param name Name of the session
 * @param filepath Path of the output file
 */
void Instrumentor::beginSession(const std::string& name,
                                const std::string& filepath)
{
    m_outputStream.open(filepath);
    writeHeader();
    m_currentSession = new InstrumentationSession{ name };
}

/**
 * @brief Finish session.
 */
void Instrumentor::endSession()
{
    if (!m_currentSession)
        return;
    writeFooter();
    m_outputStream.close();
    delete m_currentSession;
    m_currentSession = nullptr;
    m_profileCount = 0;
}

/**
 * @brief Write the profile on the output file
 * @param result Result to write
 */
void Instrumentor::writeProfile(const ProfileResult& result)
{
    if (!m_currentSession)
        return;
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_profileCount++ > 0)
        m_outputStream << ",\n";

    std::string name = result.name;
    std::replace(name.begin(), name.end(), '"', '\'');
    std::replace(name.begin(), name.end(), '\n', ' ');

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

/**
 * @brief Write the header of the output file
 */
void Instrumentor::writeHeader()
{
    m_outputStream << "{\"otherData\": {},\"traceEvents\":[";
    m_outputStream.flush();
}

/**
 * @brief Write the footer of the output file
 */
void Instrumentor::writeFooter()
{
    m_outputStream << "]}";
    m_outputStream.flush();
}
