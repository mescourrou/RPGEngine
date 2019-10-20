#pragma once

// STL
#include <vector>

// Project
#include <BaseObject.hpp>
#include <BaseException.hpp>
#include <Event.hpp>

// Extern libs
#include <SimpleIni.h>

#ifdef RPG_BUILD_TEST
#include <gtest/gtest.h>
#endif

/**
 * @namespace config
 * @brief Group configuration and global classes
 */
namespace config
{

CREATE_EXCEPTION_CLASS(Config,
                       ADD_EXCEPTION_CODE(FILE_LOADING))

#ifdef RPG_BUILD_TEST
class ConfigTest;
#endif

/**
 * @brief File based configuration class
 */
class Config : public BaseObject
{
    DECLARE_BASEOBJECT(Config)
#ifdef RPG_BUILD_TEST
    friend class config::ConfigTest;
#endif
  public:
    Config();
    Config(const std::string& filename);
    ~Config() override = default;

    virtual bool loadFile(const std::string& filename) noexcept final;
    virtual bool saveToFile(std::string filename = "") final;

    virtual std::string getValue(const std::string& section,
                                 const std::string& key) const final;
    virtual std::string getValue(const std::string& key) const;

    virtual std::vector<std::string> getAllValues(const std::string& section,
            const std::string& key) const final;

    virtual std::vector<std::string> getAllSections() const final;

    virtual bool setValue(const std::string& section, const std::string& key,
                          const std::string& value, bool forceRemplace = true) final;

    events::Event<void>
    signalConfigUpdated;        ///< Signal emitted when the config is saved

  protected:
    CSimpleIniCaseA m_iniFile; ///< INI file load in memory
    std::string m_filename;    ///< Filename used to load the config
};



} // namespace config
