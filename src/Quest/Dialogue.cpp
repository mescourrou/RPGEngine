#include "Dialogue.hpp"
#include <Database.hpp>
#include <Model.hpp>
#include <InstrumentationTimer.hpp>
#include <Query.hpp>
#include <glog/logging.h>
#include <VerbosityLevels.hpp>

namespace quest
{

/**
 * @brief Load the Dialogue starting by the id @a firstLineID .
 *
 * @warning You need to give the first line id of the Dialogue, the one stored in the 'Dialog' table
 * @param firstLineID First DialogueLine ID to start with.
 * @param db Database to load from.
 * @return Return a reference on the current object.
 */
Dialogue& Dialogue::loadFromDatabase(unsigned int firstLineID,
                                     std::shared_ptr<databaseTools::Database> db)
{
    PROFILE_FUNCTION();
    VLOG(verbosityLevel::FUNCTION_CALL) << "Dialogue loadFromDatabase " <<
                                        firstLineID;
    namespace Model = database::Model::Quest::Dialog;
    using namespace databaseTools;

    if (!db)
        throw DialogueException("No database given.",
                                BaseException::MISSING_DATABASE);

    if (!verifyDatabaseModel(db))
        throw DialogueException("Wrong database model.", BaseException::BAD_MODEL);

    auto result = db->query(Query::createQuery<Query::SELECT>(Model::TABLE, db)
                            .column(Model::FK_NPC_NAME)
                            .where(Query::Column(Model::TABLE, Model::FK_DIALOG_LINE_ID), Query::EQUAL,
                                   std::to_string(firstLineID))
                           );
    if (Database::isQuerySuccessfull(result) && result.size() > 1)
    {
        m_characterName = result.at(1).at(Model::FK_NPC_NAME);
    }
    loadDialogueLineRecursive(firstLineID, db);
    m_firstLineId = firstLineID;
    return *this;
}

/**
 * @brief Load all the Dialogue s of a NPC, defined by its name.
 * @param NPCName Name of the NPC.
 * @param db Database to load from.
 * @return List of Dialogue , currently ordored by first DialogueLine ID.
 */
std::vector<std::shared_ptr<Dialogue>> Dialogue::loadFromDatabase(
                                        const std::string& NPCName,
                                        std::shared_ptr<databaseTools::Database> db)
{
    PROFILE_FUNCTION();
    namespace Model = database::Model::Quest::Dialog;
    using namespace databaseTools;

    if (!db)
        throw DialogueException("No database given.",
                                BaseException::MISSING_DATABASE);

    if (!verifyDatabaseModel(db))
        throw DialogueException("Wrong database model.", BaseException::BAD_MODEL);

    auto result = db->query(Query::createQuery<Query::SELECT>(Model::TABLE, db)
                            .column(Model::FK_DIALOG_LINE_ID)
                            .where(Model::FK_NPC_NAME, Query::EQUAL, NPCName)
                            .sort(Model::FK_DIALOG_LINE_ID, true)
                           );

    if (result.size() <= 1)
        return {};

    std::vector<std::shared_ptr<Dialogue>> dialogueList;
    for (size_t i = 1; i < result.size(); i++)
    {
        Dialogue d;
        d.loadFromDatabase(std::atoi(result.at(i).at(Model::FK_DIALOG_LINE_ID).c_str()),
                           db);
        VLOG(verbosityLevel::VERIFICATION_LOG) << "Dialogue starting by '" <<
                                               d.firstLine().lock()->line() << "', id '" << std::atoi(result.at(i).at(
                                                       Model::FK_DIALOG_LINE_ID).c_str()) << "', loaded";
        dialogueList.push_back(std::make_shared<Dialogue>(d));
    }

    return dialogueList;
}

/**
 * @brief Verify that the given database has the necessary tables.
 *
 * We check if the table database::Model::Quest::Dialog::TABLE exists and has the good fields.
 * Then, we check DialogueLine::verifyDatabaseModel .
 *
 * @param db Database to verify.
 * @return True if the database contains the needed tables.
 */
bool Dialogue::verifyDatabaseModel(std::shared_ptr<databaseTools::Database> db)
{
    PROFILE_FUNCTION();
    namespace Model = database::Model::Quest::Dialog;
    using namespace databaseTools;

    if (!db)
        throw DialogueException("No database given.",
                                BaseException::MISSING_DATABASE);
    if (!db->isTable(Model::TABLE))
        return false;
    auto columnList = db->columnList(Model::TABLE);

    unsigned short goodColumns = 0;
    for (const std::string& column : columnList)
    {
        if (column == Model::FK_NPC_NAME)
            goodColumns++;
        else if (column == Model::FK_DIALOG_LINE_ID)
            goodColumns++;
        else
            return false;
    }

    if (goodColumns != 2)
        return false;
    return DialogueLine::verifyDatabaseModel(db);
}

/**
 * @brief Create the needed tables on the database.
 * @param db Database to populate.
 * @return Return true if the populate process was successfull.
 */
bool Dialogue::createDatabaseModel(std::shared_ptr<databaseTools::Database> db)
{
    PROFILE_FUNCTION();
    namespace Model = database::Model::Quest::Dialog;
    using namespace databaseTools;
    if (!db)
        throw DialogueException("No database given.",
                                BaseException::MISSING_DATABASE);

    db->query(Query::createQuery<Query::CREATE>(Model::TABLE, db)
              .ifNotExists()
              .column(Model::FK_NPC_NAME, DataType::BLOB,
                      database::Model::NPC::TABLE,
                      database::Model::NPC::NAME)
              .column(Model::FK_DIALOG_LINE_ID, DataType::INTEGER,
                      database::Model::Quest::DialogLine::TABLE,
                      database::Model::Quest::DialogLine::ID)
              .constraint(Model::FK_NPC_NAME, Query::PRIMARY_KEY)
              .constraint(Model::FK_DIALOG_LINE_ID, Query::PRIMARY_KEY)
             );
    return DialogueLine::createDatabaseModel(db) && verifyDatabaseModel(db);
}

/**
 * @brief Load a DialogueLine and the successor of it, recursively.
 * @param id Id of the DialogueLine to start with.
 * @param db Database to load from.
 */
void Dialogue::loadDialogueLineRecursive(unsigned int id,
        std::shared_ptr<databaseTools::Database> db)
{
    DialogueLine line;
    line.loadFromDatabase(id, db);

    namespace ModelGraph = database::Model::Quest::DialogGraph;
    using namespace databaseTools;

    auto result = db->query(Query::createQuery<Query::SELECT>(ModelGraph::TABLE, db)
                            .column(ModelGraph::FK_AFTER_ID).column(ModelGraph::CHARACTER_LINE)
                            .where(Query::Column(ModelGraph::TABLE, ModelGraph::FK_BEFORE_ID), Query::EQUAL,
                                   std::to_string(id)));

    if (result.size() > 1) // If there are lines after
    {
        for (size_t i = 1; i < result.size(); i++)
        {
            unsigned int nextId = std::atoi(result.at(i).at(
                                                ModelGraph::FK_AFTER_ID).c_str());
            if (m_dialogueLineStorage.count(nextId) == 0)
                loadDialogueLineRecursive(nextId, db);
            /**
             * @todo load action
             */
            std::string characterLine = result.at(i).at(ModelGraph::CHARACTER_LINE);
            if (characterLine == "NULL")
                characterLine = "";
            line.addChoice(characterLine, m_dialogueLineStorage[nextId], {});
        }
    }

    m_dialogueLineStorage[id] = std::make_shared<DialogueLine>(line);

}

}

