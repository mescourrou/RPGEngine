#include "NPC.hpp"

namespace character {

NPC::NPC(std::string name, std::shared_ptr<config::Context> context):
    Character (std::move(name), context)
{

}

bool NPC::loadFromDatabase(std::shared_ptr<database::Database> db)
{

}

bool NPC::verifyDatabaseModel(std::shared_ptr<database::Database> db)
{

}

bool NPC::createDatabaseModel(std::shared_ptr<database::Database> db)
{

}

void NPC::updatePosition()
{

}


} // namespace character
