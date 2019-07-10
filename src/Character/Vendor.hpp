#pragma once

// Project
#include <NPC.hpp>
#include <Inventory.hpp>

namespace character {

class Vendor : public NPC
{
public:
    Vendor() = delete;
    Vendor(std::string name, std::shared_ptr<config::Context> context);
    ~Vendor() override = default;

    const std::weak_ptr<object::Inventory> seeInventory() const;
    void sell(const std::string &objectName, Character& buyer);
    void sell(unsigned int objectInventoryId, Character& buyer);
    void buy(const object::Object &object, Character& seller);
};

} // namespace character
