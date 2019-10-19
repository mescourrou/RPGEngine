#include "Config.hpp"
// I/O
#include <iostream>
#include <fstream>
#include <memory>

// standard library
#include <string>
#include <cstdlib>

// Project
#include <VerbosityLevels.hpp>

// Extern libs
#include <glog/logging.h>

namespace config
{

/**
 * @brief Default constructor
 */
Config::Config()
{
    VLOG(verbosityLevel::OBJECT_CREATION) << "Creating " << className() << " => " <<
                                          this;
}

/**
 * @brief Constructor of Config
 * @param filename File to load
 */
Config::Config(const std::string& filename)
{
    VLOG(verbosityLevel::OBJECT_CREATION) << "Creating " << className() << " => " <<
                                          this;
    if (!loadFile(filename))
        throw ConfigException("Impossible to load the config file");
}

/**
 * @brief Load ini file into configuration
 * @param[in] filename Path name
 * @return Return false if the file couldn't be loaded
 */
bool Config::loadFile(const std::string& filename) noexcept
{
    VLOG(verbosityLevel::FUNCTION_CALL) << "Call loadFile(" << filename << ")";
    m_iniFile.SetUnicode();
    m_iniFile.SetMultiKey();
    if (m_iniFile.LoadFile(filename.c_str()) < 0)
    {
        LOG(ERROR) << "Impossible to load " << filename << " conf file";
        return false;
    }
    m_filename = filename;

    LOG(INFO) << "Loading " << filename << " conf file";

    return true;
}

/**
 * @brief Save the elements to the filename given
 *
 * Emit the signal Config::signalConfigUpdated
 * @param filename File to save into. If nothing given, it will save in the loaded file
 * @return Return true if the save was successfull
 */
bool Config::saveToFile(std::string filename)
{
    if (filename.empty())
        filename = m_filename;
    if (m_iniFile.SaveFile(filename.c_str()) < 0)
    {
        LOG(ERROR) << "Impossible to save to " << filename << " conf file";
        return false;
    }
    signalConfigUpdated.trigger();
    LOG(INFO) << "Saving to " << filename << " successfull";
    return true;
}

/**
 * @brief Get the value targetted by the section and the key given
 * @param[in] section Config section
 * @param[in] key Config key
 * @return Return the value in string, even for numbers. Empty string if not found
 */
std::string Config::getValue(const std::string& section,
                             const std::string& key) const
{
    bool multipleValues = false;
    const char* rawValue = m_iniFile.GetValue(section.c_str(), key.c_str(), nullptr,
                           &multipleValues);

    if (!rawValue)
        return {};
    if (multipleValues)
        LOG(WARNING) << "Key '" << key << "' in section '" << section <<
                     "' may have multiple values";
    return std::string(rawValue);

}

/**
 * @brief Get the value of the key given. Search in the 1st section.
 *
 * The purpose of this method is to be used in file without sections
 *
 * @param[in] key Key to search
 * @return Value of the key. Empty string if not found
 */
std::string Config::getValue(const std::string& key) const
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
std::vector<std::string> Config::getAllValues(const std::string& section,
        const std::string& key) const
{
    std::vector<std::string> ret;
    CSimpleIniCaseA::TNamesDepend values;
    m_iniFile.GetAllValues(section.c_str(), key.c_str(), values);

    // sort the values into the original load order
    values.sort(CSimpleIniCaseA::Entry::LoadOrder());

    // output all of the items
    CSimpleIniCaseA::TNamesDepend::const_iterator i;
    for (i = values.begin(); i != values.end(); ++i)
    {
        ret.push_back(i->pItem);
    }
    return ret;
}

/**
 * @brief Get the list of the sections of the file
 */
std::vector<std::string> Config::getAllSections() const
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

/**
 * @brief Set the couple section:key value in the file. To don't use section, set the section field to ""
 * @param section Section name. "" for no section
 * @param key Key name
 * @param value Value to set
 * @param forceRemplace (optionnal, true by default) Force the remplacement of the value.
 * @return Return false if the value couldn't be set
 */
bool Config::setValue(const std::string& section, const std::string& key,
                      const std::string& value, bool forceRemplace)
{
    VLOG(verbosityLevel::FUNCTION_CALL) << "setValue (" << section << ", " << key <<
                                        ", " << value << ", " << forceRemplace << ")";
    int rc = m_iniFile.SetValue(section.c_str(), key.c_str(), value.c_str(),
                                nullptr, forceRemplace);
    if (rc < 0)
    {
        LOG(ERROR) << "Error during setting value " << section << ":" << key;
        return false;
    }
    return true;
}

} // namespace config
