#pragma once

// Project
#include "general_config.hpp"
#include <BaseObject.hpp>

#ifdef RPG_BUILD_TEST
#include <gtest/gtest.h>
#endif

namespace object
{

#ifdef RPG_BUILD_TEST
class ObjectTest;
#endif

class Object : public BaseObject
{
#ifdef RPG_BUILD_TEST
    friend class object::ObjectTest;
#endif
public:
    Object() = default;
    ~Object() override = default;

    Object(const Object& copy) = default;
    Object(Object&& moved) = default;
    virtual Object& operator=(const Object& copy) = default;
    virtual Object& operator=(Object&& moved) = default;

    // Getters
    std::string className() const noexcept override { return "Object"; }
    virtual std::string name() const noexcept final { return m_name; }
    virtual std::string description() const noexcept  final { return m_description; }


    // Setters
    virtual void setName(const std::string& name) noexcept final { m_name = name; }
    virtual void setDescription(const std::string& description) noexcept final { m_description = description; }


protected:
    std::string m_name = "Unkown object";
    std::string m_description;
    unsigned int m_value;
};

// I/O
std::ostream& operator<<(std::ostream& stream, const Object& object);

} // namespace object

