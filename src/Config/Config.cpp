#include "Config.hpp"
// I/O
#include <iostream>
#include <fstream>
#include <memory>

// standard library
#include <string>
#include <cstdlib>

// Extern libs
#include <glog/logging.h>

/**
 * @brief Constructor of Config
 * @param filename File to load
 */
config::Config::Config(const std::string &filename)
{
    loadFile(filename);
}

/**
 * @brief Load ini file into configuration
 * @param filename Path name
 * @return Return false if the file couldn't be loaded
 */
bool config::Config::loadFile(const std::string &filename) noexcept
{
    if (m_iniFile.LoadFile(filename.c_str()) < 0)
        return false;

    LOG(INFO) << "Loading " << filename << " conf file";

    return true;
}

/**
 * @brief Get the value targetted by the section and the key given
 * @param section Config section
 * @param key Config key
 * @return Return the value in string, even for numbers. Empty string if not found
 */
std::string config::Config::getValue(const std::string &section, const std::string &key) const
{
    const char* rawValue = m_iniFile.GetValue(section.c_str(), key.c_str(), nullptr);

    if (!rawValue)
        return {};
    return std::string(rawValue);

}

/**
 * @brief Get the value of the key given. Search in the 1st section.
 *
 * The purpose of this method is to be used in file without sections
 *
 * @param key Key to search
 * @return Value of the key. Empty string if not found
 */
std::string config::Config::getValue(const std::string &key) const
{
    CSimpleIniA::TNamesDepend sections;
    m_iniFile.GetAllSections(sections);

    std::string ret;
    while (!sections.empty())
    {
        ret = getValue(sections.front().pItem, key);
        if (!ret.empty())
            return ret;
        sections.pop_front();
    }

    return {};

}
