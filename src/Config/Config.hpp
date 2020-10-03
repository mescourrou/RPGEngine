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

CREATE_EXCEPTION_CLASS(Config);

#ifdef RPG_BUILD_TEST
class ConfigTest;
#endif

/**
 * @brief File based configuration class
 */
class Config : public BaseObject
{
    DECLARE_BASEOBJECT(Config);
#ifdef RPG_BUILD_TEST
    friend class config::ConfigTest;
#endif
  public:
    Config();
    explicit Config(const std::string& filename);
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

    ADD_EVENT(SignalConfigUpdated,
              void); ///< Signal emitted when the config is saved

  private:
    /// INI file load in memory
    CSimpleIniCaseA m_iniFile;
    /// Filename used to load the config
    std::string m_filename;
};



} // namespace config
