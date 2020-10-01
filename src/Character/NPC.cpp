#include "NPC.hpp"
#include <Database.hpp>
#include <Model.hpp>
#include <Query.hpp>
#include <InstrumentationTimer.hpp>

namespace character
{

/**
 * @brief Constructor
 * @param name Name of the NPC
 * @param context Context to use
 */
NPC::NPC(const std::string& name, std::shared_ptr<config::Context> context):
    Character (name, context)
{

}

/**
 * @brief Load the NPC from the database
 * @param db Database to use
 * @return Return true if the loading was successfull
 */
bool NPC::loadFromDatabase(std::shared_ptr<databaseTools::Database> db)
{
    PROFILE_FUNCTION();
    namespace Model = database::Model::NPC;
    using namespace databaseTools;
    if (!verifyDatabaseModel(db))
        throw CharacterException("The database model is not correct",
                                 BaseException::BAD_MODEL);
    // Verify if the name match a NPC
    auto result = db->query(Query::createQuery<Query::SELECT>(Model::TABLE, db)
                            .where(Model::NAME, Query::EQUAL, name()));
    if (!Database::isQuerySuccessfull(result))
        return false;
    if (result.size() <= 1) // No result
        return false;

    if (!Character::loadFromDatabase(db))
        return false;

    m_dialogues = quest::Dialogue::loadFromDatabase(name(), db);

    return true;
}

/**
 * @brief Verify the database model related to the NPC : NPC and NPCPath tables
 * @param db Database to verify
 * @return Return true if the model is correct
 */
bool NPC::verifyDatabaseModel(std::shared_ptr<databaseTools::Database> db)
{
    PROFILE_FUNCTION();
    using namespace databaseTools;
    if (!db)
        throw CharacterException("No database given.",
                                 BaseException::MISSING_DATABASE);
    return  verifyNPCModel(db) &&
            verifyNPCPathModel(db);
}

/**
 * @brief Create the tables needed by the NPC class
 * @param db Database to populate
 * @return Return true if all went well
 */
bool NPC::createDatabaseModel(std::shared_ptr<databaseTools::Database> db)
{
    PROFILE_FUNCTION();

    using namespace databaseTools;
    if (!db)
        throw CharacterException("No database given.",
                                 BaseException::MISSING_DATABASE);
    namespace ModelNPC = database::Model::NPC;
    db->query(Query::createQuery<Query::CREATE>(ModelNPC::TABLE, db).ifNotExists()
              .column(ModelNPC::NAME).constraint(ModelNPC::NAME, Query::PRIMARY_KEY)
              .column(ModelNPC::TYPE, DataType::INTEGER));

    namespace ModelNPCPath = database::Model::NPCPath;
    db->query(Query::createQuery<Query::CREATE>(ModelNPCPath::TABLE,
              db).ifNotExists()
              .column(ModelNPCPath::FK_NPC_NAME, DataType::BLOB, database::Model::NPC::TABLE,
                      database::Model::NPC::NAME)
              .constraint(ModelNPCPath::FK_NPC_NAME, Query::PRIMARY_KEY)
              .column(ModelNPCPath::X, DataType::INTEGER)
              .column(ModelNPCPath::Y, DataType::INTEGER)
              .column(ModelNPCPath::Z, DataType::INTEGER));

    return verifyDatabaseModel(db);
}

/**
 * @brief Compute the new position of the NPC
 */
void NPC::updatePosition()
{
    // Patrol to implement
}

/**
 * @brief Verify the model of NPC table
 * @param db Databasse to check
 * @return Return true if the model is correct
 */
bool NPC::verifyNPCModel(std::shared_ptr<databaseTools::Database> db)
{
    PROFILE_FUNCTION();
    namespace Model = database::Model::NPC;
    if (!db->isTable(Model::TABLE))
        return false;
    auto columnList = db->columnList(Model::TABLE);

    unsigned short goodColumns = 0;
    for (auto& column : columnList)
    {
        if (column == Model::NAME)
            goodColumns++;
        else if (column == Model::TYPE)
            goodColumns++;
    }

    if (goodColumns != 2)
        return false;

    return  true;
}

/**
 * @brief Verify the NPCPath table model
 * @param db Database to check$
 *
 * @return Return true if the model is correct
 */
bool NPC::verifyNPCPathModel(std::shared_ptr<databaseTools::Database> db)
{
    PROFILE_FUNCTION();
    namespace Model = database::Model::NPCPath;
    if (!db->isTable(Model::TABLE))
        return false;
    auto columnList = db->columnList(Model::TABLE);


    unsigned short goodColumns = 0;
    for (auto& column : columnList)
    {
        if (column == Model::ID)
            goodColumns++;
        else if (column == Model::FK_NPC_NAME)
            goodColumns++;
        else if (column == Model::X)
            goodColumns++;
        else if (column == Model::Y)
            goodColumns++;
        else if (column == Model::Z)
            goodColumns++;
    }

    if (goodColumns != 5)
        return false;

    return true;
}


} // namespace character
