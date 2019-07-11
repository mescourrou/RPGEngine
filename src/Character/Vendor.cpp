#include "Vendor.hpp"

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
void Vendor::sell(const std::string& objectName, Character &buyer)
{

}

/**
 * @brief The vendor sells the requested object to the buyer
 * @param[in] objectInventoryId Iventory id of the object to sell
 * @param[out] buyer Reference on the buyer to put the object on its inventory
 */
void Vendor::sell(unsigned int objectInventoryId, Character &buyer)
{

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
