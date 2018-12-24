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
    m_iniFile.SetUnicode();
    m_iniFile.SetMultiKey();
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
    bool multipleValues = false;
    const char* rawValue = m_iniFile.GetValue(section.c_str(), key.c_str(), nullptr, &multipleValues);

    if (!rawValue)
        return {};
    if (multipleValues)
        LOG(WARNING) << "Key '" << key << "' in section '" << section << "' may have multiple values";
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
    CSimpleIniCaseA::TNamesDepend sections;
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

/**
 * @brief Get all the values of a key
 * @param [in] section Section to look in
 * @param [in] key Key to look for
 * @return List of the values. Empty list if the pair section-key wasn't found
 */
std::vector<std::string> config::Config::getAllValues(const std::string &section, const std::string &key) const
{
    std::vector<std::string> ret;
    CSimpleIniCaseA::TNamesDepend values;
    m_iniFile.GetAllValues(section.c_str(), key.c_str(), values);

    // sort the values into the original load order
    values.sort(CSimpleIniCaseA::Entry::LoadOrder());

    // output all of the items
    CSimpleIniCaseA::TNamesDepend::const_iterator i;
    for (i = values.begin(); i != values.end(); ++i) {
        ret.push_back(i->pItem);
    }
    return ret;
}

/**
 * @brief Get the list of the sections of the file
 */
std::vector<std::string> config::Config::getAllSections() const
{
    std::vector<std::string> sectionList;
    CSimpleIniCaseA::TNamesDepend sections;
    m_iniFile.GetAllSections(sections);

    for (auto& section : sections)
    {
        sectionList.push_back(section.pItem);
    }
    return sectionList;
}
