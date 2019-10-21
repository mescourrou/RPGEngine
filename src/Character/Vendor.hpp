#pragma once

// Project
#include <NPC.hpp>
#include <Inventory.hpp>

namespace character
{

/**
 * @brief Class managing NPCs which can buy and sell objects
 *
 * It simply buy and sell what it has on its inventory
 */
class Vendor : public NPC
{
    DECLARE_BASEOBJECT(Vendor)
  public:
    Vendor() = delete;
    Vendor(const std::string &name, std::shared_ptr<config::Context> context);
    /// @brief Default constructor
    ~Vendor() override = default;

    bool loadFromDatabase(std::shared_ptr<database::Database> db) override;

    const std::weak_ptr<object::Inventory> seeInventory() const;
    bool sell(const std::string& objectName, Character& buyer);
    bool sell(unsigned int objectInventoryId, Character& buyer);
    bool buy(const std::string& objectName, Character& seller);
    bool buy(unsigned int objectInventoryId, Character& seller);

};

} // namespace character
