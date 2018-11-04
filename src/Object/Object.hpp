#pragma once

// Project
#include "general_config.hpp"
#include <BaseObject.hpp>
#include <Money.hpp>

#ifdef RPG_BUILD_TEST
#include <gtest/gtest.h>
#endif

/** @namespace object
 * @brief Group the classes usefull for object using
 */
namespace object
{

#ifdef RPG_BUILD_TEST
class ObjectTest;
#endif
/**
 * @brief Object which can be manipulated by the Caracter
 */
class Object : public BaseObject
{
#ifdef RPG_BUILD_TEST
    friend class object::ObjectTest;
#endif
public:
    Object() = default;
    Object(std::string name);
    ~Object() override = default;

    Object(const Object& copy) = default;
    Object(Object&& moved) = default;
    virtual Object& operator=(const Object& copy) = default;
    virtual Object& operator=(Object&& moved) = default;

    // Getters
    std::string className() const noexcept override { return "Object"; }
    /// @brief Get the object name
    virtual std::string name() const noexcept final { return m_name; }
    /// @brief Get the object description
    virtual std::string description() const noexcept  final { return m_description; }
    /// @brief get the value of the object
    virtual Money value() const noexcept final { return m_value; }


    // Setters
    /**
     * @brief Set the object name
     * @param name New name of the object
     */
    virtual void setName(const std::string& name) noexcept final { m_name = name; }
    /**
     * @brief Modify the object description
     * @param description New description of the object
     */
    virtual void setDescription(const std::string& description) noexcept final { m_description = description; }
    /**
     * @brief Set the new value of the object
     * @param value New value
     */
    virtual void setValue(const Money& value) final { m_value = value; }


protected:
    std::string m_name = "Unkown object"; ///< Name of the object
    std::string m_description; ///< Description of the object
    Money m_value; ///< Value of the object
};

// I/O
std::ostream& operator<<(std::ostream& stream, const Object& object);

} // namespace object

