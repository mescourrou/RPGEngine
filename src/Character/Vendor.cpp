#include "Vendor.hpp"
#include <Object.hpp>
#include <Database.hpp>
#include <Query.hpp>
#include <Model.hpp>

namespace character
{

/**
 * @brief Constructor
 * @param name Name of the Vendor
 * @param context Context used
 */
Vendor::Vendor(std::string name, std::shared_ptr<config::Context> context) :
    NPC(std::move(name), context)
{

}


bool Vendor::loadFromDatabase(std::shared_ptr<database::Database> db)
{
    if (!NPC::loadFromDatabase(db))
        return false;
    using namespace database;
    namespace Model = Model::NPC;
    auto result = db->query(Query::createQuery<Query::SELECT>(Model::TABLE, db)
                            .where(Model::NAME, Query::EQUAL, m_name)
                            .column(Model::TYPE));
    if (std::stoi(result.at(1).at(Model::TYPE)) != Model::VENDOR)
        return false;

    return true;
}

/**
 * @brief See the objects which can be sold
 * @return Inventory of the Vendor
 */
const std::weak_ptr<object::Inventory> Vendor::seeInventory() const
{
    return m_inventory;
}

/**
 * @brief The vendor sells the requested object to the buyer
 * @param[in] objectName Object requested
 * @param[out] buyer Reference on the buyer to put the object on its inventory
 */
bool Vendor::sell(const std::string& objectName, Character& buyer)
{
    if (!m_inventory->get(objectName))
        return false;
    if (buyer.inventory().lock()->pullMoney(m_inventory->get(objectName)->value()))
    {
        m_inventory->addMoney(m_inventory->get(objectName)->value());
        auto object = m_inventory->pop(objectName);

        buyer.inventory().lock()->push(object);
        return true;
    }
    return false;
}

/**
 * @brief The vendor sells the requested object to the buyer
 * @param[in] objectInventoryId Iventory id of the object to sell
 * @param[out] buyer Reference on the buyer to put the object on its inventory
 */
bool Vendor::sell(unsigned int objectInventoryId, Character& buyer)
{
    if (!m_inventory->get(objectInventoryId))
        return false;
    if (buyer.inventory().lock()->pullMoney(m_inventory->get(
            objectInventoryId)->value()))
    {
        m_inventory->addMoney(m_inventory->get(objectInventoryId)->value());
        auto object = m_inventory->pop(objectInventoryId);

        buyer.inventory().lock()->push(object);
        return true;
    }
    return false;

}

/**
 * @brief The vendor buy the object from the seller
 * @param objectName Object to buy
 * @param seller Seller
 */
bool Vendor::buy(const std::string& objectName, Character& seller)
{
    if (!seller.inventory().lock()->get(objectName))
        return false;
    if (m_inventory->pullMoney(seller.inventory().lock()->get(objectName)->value()))
    {
        seller.inventory().lock()->addMoney(seller.inventory().lock()->get(
                                                objectName)->value());
        auto object = seller.inventory().lock()->pop(objectName);

        m_inventory->push(object);
        return true;
    }
    return false;
}

/**
 * @brief The vendor buy the object from the seller
 * @param objectInventoryId Id of the object on the seller inventory
 * @param seller Seller
 */
bool Vendor::buy(unsigned int objectInventoryId, Character& seller)
{
    if (!seller.inventory().lock()->get(objectInventoryId))
        return false;
    if (m_inventory->pullMoney(seller.inventory().lock()->get(
                                   objectInventoryId)->value()))
    {
        seller.inventory().lock()->addMoney(seller.inventory().lock()->get(
                                                objectInventoryId)->value());
        auto object = seller.inventory().lock()->pop(objectInventoryId);

        m_inventory->push(object);
        return true;
    }
    return false;
}

} // namespace character
