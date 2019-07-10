#include "Vendor.hpp"

namespace character {

Vendor::Vendor(std::string name, std::shared_ptr<config::Context> context) :
    NPC(std::move(name), context)
{

}

const std::weak_ptr<object::Inventory> Vendor::seeInventory() const
{
    return m_inventory;
}

void Vendor::sell(const std::string& objectName, Character &buyer)
{

}

void Vendor::sell(unsigned int objectInventoryId, Character &buyer)
{

}

void Vendor::buy(const object::Object& object, Character &seller)
{

}

} // namespace character
