#pragma once

// Project
#include "general_config.hpp"
#include <BaseObject.hpp>
#include <Money.hpp>
#include <BaseException.hpp>

#ifdef RPG_BUILD_TEST
#include <gtest/gtest.h>
#endif

namespace databaseTools
{
class Database;
}

/** @namespace object
 * @brief Group the classes usefull for object using
 */
namespace object
{

CREATE_EXCEPTION_CLASS(Object,
                       ADD_EXCEPTION_CODE(UNKNOWN_OBJECT_TYPE))

#ifdef RPG_BUILD_TEST
class ObjectTest;
#endif
/**
 * @brief Object which can be manipulated by the Caracter
 */
class Object : public BaseObject
{
    DECLARE_BASEOBJECT(Object)
#ifdef RPG_BUILD_TEST
    friend class object::ObjectTest;
#endif
  public:
    /// @brief Default constructor
    Object() = default;
    explicit Object(std::string name);
    /// @brief Default destructor
    ~Object() override = default;

    Object(const Object& copy) = default;
    Object(Object&& moved) = default;
    Object& operator=(const Object& copy) = default;
    Object& operator=(Object&& moved) = default;

    virtual bool loadFromDatabase(std::shared_ptr<databaseTools::Database> db);
    static std::shared_ptr<Object> createFromDatabase(const std::string& name,
            std::shared_ptr<databaseTools::Database> db);

    // Getters
    /// @brief Get the object name
    virtual std::string name() const noexcept final
    {
        return m_name;
    }
    /// @brief Get the object description
    virtual std::string description() const noexcept  final
    {
        return m_description;
    }
    /// @brief get the value of the object
    virtual Money value() const noexcept final
    {
        return m_value;
    }


    // Setters
    /**
     * @brief Set the object name
     * @param name New name of the object
     */
    virtual void setName(const std::string& name) noexcept final
    {
        m_name = name;
    }
    /**
     * @brief Modify the object description
     * @param description New description of the object
     */
    virtual void setDescription(const std::string& description) noexcept final
    {
        m_description = description;
    }
    /**
     * @brief Set the new value of the object
     * @param value New value
     */
    virtual void setValue(const Money& value) final
    {
        m_value = value;
    }


    static bool verifyDatabaseModel(std::shared_ptr<databaseTools::Database> db);
    static bool createDatabaseModel(std::shared_ptr<databaseTools::Database> db);

  private:
    /// Name of the object
    std::string m_name = "Unkown object";
    /// Description of the object
    std::string m_description;
    /// Value of the object
    Money m_value;

};

// I/O
std::ostream& operator<<(std::ostream& stream, const Object& object);

} // namespace object

