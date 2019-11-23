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
Vendor::Vendor(const std::string& name,
               std::shared_ptr<config::Context> context) :
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
                            .where(Model::NAME, Query::EQUAL, name())
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
    return inventory();
}

/**
 * @brief The vendor sells the requested object to the buyer
 * @param[in] objectName Object requested
 * @param[out] buyer Reference on the buyer to put the object on its inventory
 */
bool Vendor::sell(const std::string& objectName, Character& buyer)
{
    if (!privateInventory()->get(objectName))
        return false;
    if (buyer.inventory().lock()->pullMoney(privateInventory()->get(
            objectName)->value()))
    {
        privateInventory()->addMoney(privateInventory()->get(objectName)->value());
        auto object = privateInventory()->pop(objectName);

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
    if (!privateInventory()->get(objectInventoryId))
        return false;
    if (buyer.inventory().lock()->pullMoney(privateInventory()->get(
            objectInventoryId)->value()))
    {
        privateInventory()->addMoney(privateInventory()->get(
                                         objectInventoryId)->value());
        auto object = privateInventory()->pop(objectInventoryId);

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
    if (privateInventory()->pullMoney(seller.inventory().lock()->get(
                                          objectName)->value()))
    {
        seller.inventory().lock()->addMoney(seller.inventory().lock()->get(
                                                objectName)->value());
        auto object = seller.inventory().lock()->pop(objectName);

        privateInventory()->push(object);
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
    if (privateInventory()->pullMoney(seller.inventory().lock()->get(
                                          objectInventoryId)->value()))
    {
        seller.inventory().lock()->addMoney(seller.inventory().lock()->get(
                                                objectInventoryId)->value());
        auto object = seller.inventory().lock()->pop(objectInventoryId);

        privateInventory()->push(object);
        return true;
    }
    return false;
}

} // namespace character
