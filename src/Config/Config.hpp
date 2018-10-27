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

namespace config
{

#ifdef RPG_BUILD_TEST
class ConfigTest;
#endif

class Config : public BaseObject
{
#ifdef RPG_BUILD_TEST
    friend class config::ConfigTest;
#endif
public:
    Config() = default;
    Config(const std::string& filename);
    ~Config() override;

    std::string className() const override { return "Config"; }

    virtual bool loadFile(const std::string& filename) final;

    virtual std::string getValue(const std::string& section, const std::string& key) const final;
    virtual std::string getValue(const std::string& key) const;


protected:
    CSimpleIniA m_iniFile;
};



} // namespace config
