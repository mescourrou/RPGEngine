#include "Config.hpp"

config::Config::Config(const std::string &filename)
{
    loadFile(filename);
}

bool config::Config::loadFile(const std::string &filename) noexcept
{
    if (m_iniFile.LoadFile(filename.c_str()) < 0)
        return false;

    LOG(INFO) << "Loading " << filename << " conf file";

    return true;
}

std::string config::Config::getValue(const std::string &section, const std::string &key) const
{
    const char* rawValue = m_iniFile.GetValue(section.c_str(), key.c_str(), nullptr);

    if (!rawValue)
        return {};
    return std::string(rawValue);

}

std::string config::Config::getValue(const std::string &key) const
{
    CSimpleIniA::TNamesDepend sections;
    m_iniFile.GetAllSections(sections);

    std::string ret;
    while (sections.size() > 0)
    {
        ret = getValue(sections.front().pItem, key);
        if (ret != "")
            return ret;
        sections.pop_front();
    }

    return {};

}
