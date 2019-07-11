#include "Vendor.hpp"
#include <Object.hpp>

namespace character {

/**
 * @brief Constructor
 * @param name Name of the Vendor
 * @param context Context used
 */
Vendor::Vendor(std::string name, std::shared_ptr<config::Context> context) :
    NPC(std::move(name), context)
{

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
bool Vendor::sell(const std::string& objectName, Character &buyer)
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
bool Vendor::sell(unsigned int objectInventoryId, Character &buyer)
{
    if (!m_inventory->get(objectInventoryId))
        return false;
    if (buyer.inventory().lock()->pullMoney(m_inventory->get(objectInventoryId)->value()))
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
 * The signature may change
 * @param object Object to buy
 * @param seller Seller
 */
void Vendor::buy(const object::Object& object, Character &seller)
{

}

} // namespace character
