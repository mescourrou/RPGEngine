#include "NPC.hpp"
#include <Database.hpp>
#include <Model.hpp>
#include <Query.hpp>

namespace character {

NPC::NPC(std::string name, std::shared_ptr<config::Context> context):
    Character (std::move(name), context)
{

}

bool NPC::loadFromDatabase(std::shared_ptr<database::Database> db)
{
    namespace Model = database::Model::NPC;
    using namespace database;
    if (!verifyDatabaseModel(db))
        throw CharacterException("The database model is not correct", DatabaseException::BAD_MODEL);
    // Verify if the name match a NPC
    auto result = db->query(Query::createQuery<Query::SELECT>(Model::TABLE, db)
                              .where(Model::NAME, Query::EQUAL, m_name));
    if (!Database::isQuerySuccessfull(result))
        return false;
    if (result.size() <= 1) // No result
        return false;

    if (!Character::loadFromDatabase(db))
        return false;


    return true;
}

bool NPC::verifyDatabaseModel(std::shared_ptr<database::Database> db)
{
    using namespace database;
    if (!db)
        throw CharacterException("No database given.", DatabaseException::MISSING_DATABASE);
    {
        namespace Model = database::Model::NPC;
        if (!db->isTable(Model::TABLE))
            return false;
        auto columnList = db->columnList(Model::TABLE);

        unsigned short goodColumns = 0;
        for (auto& column : columnList)
        {
            if (column == Model::NAME)
                goodColumns++;
        }

        if (goodColumns != 1)
            return false;
    }
    {
        namespace Model = database::Model::NPCPath;
        if (!db->isTable(Model::TABLE))
            return false;
        auto columnList = db->columnList(Model::TABLE);

        unsigned short goodColumns = 0;
        for (auto& column : columnList)
        {
            if (column == Model::FK_NPC_NAME)
                goodColumns++;
            else if (column == Model::X)
                goodColumns++;
            else if (column == Model::Y)
                goodColumns++;
            else if (column == Model::Z)
                goodColumns++;
        }

        if (goodColumns != 4)
            return false;
    }
    return true;
}

bool NPC::createDatabaseModel(std::shared_ptr<database::Database> db)
{

    using namespace database;
    if (!db)
        throw CharacterException("No database given.", DatabaseException::MISSING_DATABASE);
    {
        namespace Model = database::Model::NPC;
        db->query(Query::createQuery<Query::CREATE>(Model::TABLE, db).ifNotExists()
                  .column(Model::NAME).constraint(Model::NAME, Query::PRIMARY_KEY));
    }
    {
        namespace Model = database::Model::NPCPath;
        db->query(Query::createQuery<Query::CREATE>(Model::TABLE, db).ifNotExists()
                  .column(Model::FK_NPC_NAME, DataType::BLOB, database::Model::NPC::TABLE, database::Model::NPC::NAME)
                  .constraint(Model::FK_NPC_NAME, Query::PRIMARY_KEY)
                  .column(Model::X, DataType::INTEGER)
                  .column(Model::Y, DataType::INTEGER)
                  .column(Model::Z, DataType::INTEGER));
    }

    return verifyDatabaseModel(db);
}

void NPC::updatePosition()
{

}


} // namespace character
