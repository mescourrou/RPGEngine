#include "Inventory.hpp"

// Std lib
#include <algorithm>

// Project
#include <Object.hpp>
#include <Database.hpp>
#include <Query.hpp>
#include <Model.hpp>
#include <VerbosityLevels.hpp>

#include <glog/logging.h>

/**
 * @brief Get a pointer on the wanted object, but keep it on the inventory
 * @param index Number of the object (start with 0)
 * @return Pointer on the object
 */
object::Inventory::Inventory()
{
    VLOG(verbosityLevel::OBJECT_CREATION) << "Creating " << className() << " => " << this;
}

std::shared_ptr<object::Object> object::Inventory::get(unsigned int index) const
{
    if (index < m_inventory.size())
    {
        unsigned int i = 0;
        for (auto object : m_inventory)
        {
            if (i == index)
                return object;

            i++;
        }
    }
    return {};
}

/**
 * @brief Get a pointer on the wanted object, but keep it on the inventory
 * @param objectName Name of the object wanted
 * @return Pointer on the object
 */
std::shared_ptr<object::Object> object::Inventory::get(const std::string &objectName) const
{
    auto objectIt = std::find_if(m_inventory.begin(), m_inventory.end(),
                                 [objectName](std::shared_ptr<Object> object) -> bool { if (object) return object->name() == objectName; return false; });
    if (objectIt != m_inventory.end())
        return *objectIt;
    return {};
}

/**
 * @brief Remove the object and return a pointer on it
 * @param index Number of the object
 * @return Pointer on the removed object
 */
std::shared_ptr<object::Object> object::Inventory::pop(unsigned int index)
{
    std::shared_ptr<object::Object> ret = get(index);
    if (ret)
    {
        m_inventory.remove(ret);
    }
    return ret;
}

/**
 * @brief Remove the object and return a pointer on it
 * @param objectName Name of the object
 * @return Pointer on the removed object
 */
std::shared_ptr<object::Object> object::Inventory::pop(const std::string &objectName)
{
    std::shared_ptr<object::Object> ret = get(objectName);
    if (ret)
    {
        m_inventory.remove(ret);
    }
    return ret;
}

/**
 * @brief Add a new object on the inventory
 * @param newObject Smart pointer of the new object
 */
void object::Inventory::push(const std::shared_ptr<object::Object>& newObject)
{
    if (newObject)
        m_inventory.emplace_back(newObject);
}

/**
 * @brief Load the inventory from the database.
 *
 * Instanciate all the objects saved in it
 * @param [in] db Database to use
 * @param [in] characterName Name of the Character owning the inventory
 * @return Return true if the loading was successfull
 */
bool object::Inventory::loadFromDatabase(std::shared_ptr<database::Database> db, const std::string characterName)
{
    namespace Model = database::Model::Inventory;
    using namespace database;
    if (!db)
        throw InventoryException("No database given.", Database::DatabaseException::MISSING_DATABASE);
    if (!verifyDatabaseModel(db))
        throw InventoryException("The database model is not correct", Database::DatabaseException::BAD_MODEL);

    // Load information from Model::TABLE => Main inventory table
    {
    auto result = db->query(Query::createQuery<Query::SELECT>(Model::TABLE, db)
                            .column(Model::MONEY).where(Model::FK_CHARACTER, Query::EQUAL, characterName));
    if (result.size() <= 1)
        return false;
    m_money = Money{static_cast<unsigned int>(std::atoi(result.at(1).at(Model::MONEY).c_str()))};
    }

    // Load the objects
    auto objectsToLoad = db->query(Query::createQuery<Query::SELECT>(Model::InventoryObjects::TABLE, db)
                                   .column(Model::InventoryObjects::QUANTITY)
                                   .column(Model::InventoryObjects::FK_OBJECT)
                                   .where(Model::InventoryObjects::FK_CHARACTER, Query::EQUAL, characterName));

    if (objectsToLoad.size() <= 1)
        return true;

    for (unsigned int j = 1; j < objectsToLoad.size(); j++)
    {
        for (unsigned int i = 0; i < static_cast<unsigned int>(std::atoi(objectsToLoad.at(j).at(Model::InventoryObjects::QUANTITY).c_str())); i++)
        {
            push(Object::createFromDatabase(objectsToLoad.at(j).at(Model::InventoryObjects::FK_OBJECT), db));
        }
    }

    return true;
}

/**
 * @brief Verify that the database contains all the information needed
 * @param [in] db Database to verify
 * @return Return true if the database is valid
 */
bool object::Inventory::verifyDatabaseModel(std::shared_ptr<database::Database> db)
{
    namespace Model = database::Model::Inventory;
    using namespace database;
    if (!db->isTable(Model::TABLE))
        return false;
    auto columnList = db->columnList(Model::TABLE);

    unsigned short goodColumns = 0;
    for (auto& column : columnList)
    {
        if (column == Model::FK_CHARACTER)
            goodColumns++;
        else if (column == Model::MONEY)
            goodColumns++;
        else
            return false;
    }
    if (goodColumns != 2)
        return false;

    if (!db->isTable(Model::InventoryObjects::TABLE))
        return false;
    columnList = db->columnList(Model::InventoryObjects::TABLE);

    goodColumns = 0;
    for (auto& column : columnList)
    {
        if (column == Model::InventoryObjects::FK_CHARACTER)
            goodColumns++;
        else if (column == Model::InventoryObjects::QUANTITY)
            goodColumns++;
        else if (column == Model::InventoryObjects::FK_OBJECT)
            goodColumns++;
        else
            return false;
    }
    if (goodColumns != 3)
        return false;

    return true;
}

bool object::Inventory::createDatabaseModel(std::shared_ptr<database::Database> db)
{
    namespace Model = database::Model::Inventory;
    using namespace database;

    if (!db)
        throw InventoryException("No database given.", database::Database::DatabaseException::MISSING_DATABASE);

    db->query(Query::createQuery<Query::CREATE>(Model::TABLE, db).ifNotExists()
              .column(Model::FK_CHARACTER, DataType::BLOB, database::Model::Character::TABLE, database::Model::Character::NAME)
              .column(Model::MONEY, DataType::INTEGER)
              .constraint(Model::FK_CHARACTER, Query::PRIMARY_KEY));

    db->query(Query::createQuery<Query::CREATE>(Model::InventoryObjects::TABLE, db).ifNotExists()
              .column(Model::InventoryObjects::FK_CHARACTER, DataType::BLOB, database::Model::Character::TABLE, database::Model::Character::NAME)
              .column(Model::InventoryObjects::QUANTITY, DataType::INTEGER)
              .column(Model::InventoryObjects::FK_OBJECT, DataType::BLOB, database::Model::Object::TABLE, database::Model::Object::NAME)
              .constraint(Model::InventoryObjects::FK_CHARACTER, Query::PRIMARY_KEY)
              .constraint(Model::InventoryObjects::FK_OBJECT, Query::PRIMARY_KEY));

    return verifyDatabaseModel(db);
}

