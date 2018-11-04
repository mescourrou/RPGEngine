#pragma once

// I/O
#include <iostream>
#include <fstream>
#include <memory>

// standard library
#include <string>
#include <cstdlib>

// Project
#include "BaseObject.hpp"

// Extern libs
#include <SimpleIni.h>
#include <glog/logging.h>

#ifdef RPG_BUILD_TEST
#include <gtest/gtest.h>
#endif

/**
 * @namespace config
 * @brief Group configuration and global classes
 */
namespace config
{

#ifdef RPG_BUILD_TEST
class ConfigTest;
#endif

/**
 * @brief File based configuration class
 */
class Config : public BaseObject
{
#ifdef RPG_BUILD_TEST
    friend class config::ConfigTest;
#endif
public:
    /// @brief Constructor
    Config() = default;
    Config(const std::string& filename);
    ~Config() override = default;

    std::string className() const noexcept override { return "Config"; }

    virtual bool loadFile(const std::string& filename) noexcept final;

    virtual std::string getValue(const std::string& section, const std::string& key) const final;
    virtual std::string getValue(const std::string& key) const;


protected:
    CSimpleIniA m_iniFile; ///< INI file load in memory
};



} // namespace config
