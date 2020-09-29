#pragma once


// Project
#include "general_config.hpp"
#include <BaseObject.hpp>
#include <BaseDatabaseObject.hpp>
#include <BaseException.hpp>
#include <Money.hpp>

#ifdef RPG_BUILD_TEST
#include <gtest/gtest.h>
#endif

namespace databaseTools
{
class Database;
}

namespace object
{

CREATE_EXCEPTION_CLASS(Inventory)

class Object;

#ifdef RPG_BUILD_TEST
class InventoryTest;
#endif

/**
 * @brief Inventory class : container for Object
 */
class Inventory : public BaseObject, public BaseDatabaseObject
{
    DECLARE_BASEOBJECT(Inventory)
#ifdef RPG_BUILD_TEST
    friend class object::InventoryTest;
    FRIEND_TEST(InventoryTest, Push);
#endif
  public:
    /// @brief Constructor
    Inventory(const std::string& characterName);
    /// @brief Destructor
    ~Inventory() override = default;

    std::shared_ptr<Object> get(unsigned int index) const;
    std::shared_ptr<Object> get(const std::string& objectName) const;
    unsigned int getNumberOf(const std::string& objectName) const;
    std::shared_ptr<Object> pop(unsigned int index);
    std::shared_ptr<Object> pop(const std::string& objectName);
    void push(const std::shared_ptr<Object>& newObject);

    bool loadFromDatabase(std::shared_ptr<databaseTools::Database> db) override;

    /// @brief Get the money contained in the inventory
    const Money& money() const
    {
        return m_money;
    }
    /// @brief Add money to the inventory
    void addMoney(const Money& m)
    {
        m_money += m;
    }
    bool pullMoney(const Money& m);

    /// @brief Return the size of the inventory
    unsigned int size() const
    {
        return m_inventory.size();
    }

    static bool verifyDatabaseModel(std::shared_ptr<databaseTools::Database> db);
    static bool createDatabaseModel(std::shared_ptr<databaseTools::Database> db);
  private:

    std::string m_characterName;
    std::list<std::shared_ptr<Object>> m_inventory; ///< List of the objects

    Money m_money;  ///< Money contained in the inventory
};

} // namespace object

