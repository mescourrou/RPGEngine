#include "DialogueLine.hpp"

#include <Database.hpp>
#include <Query.hpp>
#include <Model.hpp>
#include <InstrumentationTimer.hpp>

namespace quest
{

/**
 * @brief Construct with only the line.
 */
DialogueLine::DialogueLine(std::string line) : m_line(std::move(line))
{

}

DialogueLine::DialogueLine(DialogueLine&& move) noexcept :
    m_line(move.m_line), m_choices(move.m_choices)
{
}

/**
 * @brief Delete the actions stored.
 */
DialogueLine::~DialogueLine()
{
    for (const Choice& choice : m_choices)
    {
        if (choice.action)
            delete choice.action;
    }
}

DialogueLine& DialogueLine::operator=(DialogueLine&& move) noexcept
{
    m_line = move.m_line;
    m_choices = move.m_choices;
    return *this;
}

/**
 * @brief Load the DialogueLine of the given id from the database.
 * @param id Id of the DialogueLine.
 * @param db Database to load from.
 */
void DialogueLine::loadFromDatabase(unsigned int id,
                                    std::shared_ptr<databaseTools::Database> db)
{
    PROFILE_FUNCTION();
    namespace ModelLine = database::Model::Quest::DialogLine;
    using namespace databaseTools;

    if (!db)
        throw DialogueLineException("No database given.",
                                    BaseException::MISSING_DATABASE);

    if (!verifyDatabaseModel(db))
        throw DialogueLineException("Wrong database model.", BaseException::BAD_MODEL);

    auto result = db->query(Query::createQuery<Query::SELECT>(ModelLine::TABLE, db)
                            .where(Query::Column(ModelLine::TABLE, ModelLine::ID), Query::EQUAL,
                                   std::to_string(id))
                           );

    if (result.size() <= 1)
        return;

    m_line = result.at(1).at(ModelLine::LINE);

}

/**
 * @brief Add a player choice.
 * @param playerLine Line of the player. Can be empty.
 * @param nextLine Pointer on the next NPC line. Can be null.
 * @param action Owned pointer on the DialogueAction associated to the choice.
 */
void DialogueLine::addChoice(std::string playerLine,
                             const DialogueLine* nextLine, DialogueAction* action)
{
    m_choices.push_back(Choice{std::move(playerLine), nextLine, action});
}

/**
 * @brief Set the line.
 */
void DialogueLine::setLine(std::string line)
{
    m_line = std::move(line);
}

/**
 * @brief Get the line.
 */
const std::string& DialogueLine::getLine() const
{
    return m_line;
}

/**
 * @brief Get the player lines of the choices.
 */
std::vector<std::string> DialogueLine::getChoices() const
{
    std::vector<std::string> choices;
    for (const Choice& choice : m_choices)
    {
        choices.push_back(choice.playerLine);
    }
    return choices;
}

/**
 * @brief Select the wanted choice, execute the action associated and return the next line pointer.
 * @param index Index of the choice to select. The index is the same than in the choice list given by getChoices() .
 */
const DialogueLine* DialogueLine::selectChoice(size_t index) const
{
    const Choice& c = m_choices.at(index);
    if (c.action)
        (*c.action)();
    return c.nextLine;
}

/**
 * @brief Verify that the database contains the needed tables.
 *
 * The tables checked are :
 * - DialogLine
 * - DialogGraph
 * - DialogAction
 * @param db Database to verify.
 * @return True if the database contains all the needed tables.
 */
bool DialogueLine::verifyDatabaseModel(std::shared_ptr<databaseTools::Database>
                                       db)
{
    PROFILE_FUNCTION();
    namespace ModelDialogLine = database::Model::Quest::DialogLine;
    using namespace databaseTools;
    if (!db)
        throw DialogueLineException("No database given.",
                                    BaseException::MISSING_DATABASE);
    if (!db->isTable(ModelDialogLine::TABLE))
        return false;
    auto columnList = db->columnList(ModelDialogLine::TABLE);

    unsigned short goodColumns = 0;
    for (const std::string& column : columnList)
    {
        if (column == ModelDialogLine::ID)
            goodColumns++;
        else if (column == ModelDialogLine::LINE)
            goodColumns++;
        else if (column == ModelDialogLine::FK_ACTION_ID)
            goodColumns++;
        else
            return false;
    }

    if (goodColumns != 3)
        return false;

    namespace ModelDialogGraph = database::Model::Quest::DialogGraph;

    if (!db->isTable(ModelDialogGraph::TABLE))
        return false;
    columnList = db->columnList(ModelDialogGraph::TABLE);

    goodColumns = 0;
    for (const std::string& column : columnList)
    {
        if (column == ModelDialogGraph::FK_AFTER_ID)
            goodColumns++;
        else if (column == ModelDialogGraph::FK_BEFORE_ID)
            goodColumns++;
        else if (column == ModelDialogGraph::CHARACTER_LINE)
            goodColumns++;
        else
            return false;
    }

    if (goodColumns != 3)
        return false;

    namespace ModelDialogAction = database::Model::Quest::DialogAction;

    if (!db->isTable(ModelDialogAction::TABLE))
        return false;
    columnList = db->columnList(ModelDialogAction::TABLE);

    goodColumns = 0;
    for (const std::string& column : columnList)
    {
        if (column == ModelDialogAction::ID)
            goodColumns++;
        else
            return false;
    }

    if (goodColumns != 1)
        return false;
    return true;
}

/**
 * @brief Create the needed tables in the database.
 * @param db Database to populate.
 * @return True if the populate process was successfull.
 */
bool DialogueLine::createDatabaseModel(std::shared_ptr<databaseTools::Database>
                                       db)
{
    PROFILE_FUNCTION();
    namespace ModelDialogLine = database::Model::Quest::DialogLine;
    using namespace databaseTools;
    if (!db)
        throw DialogueLineException("No database given.",
                                    BaseException::MISSING_DATABASE);

    db->query(Query::createQuery<Query::CREATE>(ModelDialogLine::TABLE, db)
              .ifNotExists()
              .column(ModelDialogLine::ID, DataType::INTEGER).constraint(ModelDialogLine::ID,
                      Query::PRIMARY_KEY).constraint(ModelDialogLine::ID, Query::AUTOINCREMENT)
              .column(ModelDialogLine::LINE, DataType::TEXT)
              .column(ModelDialogLine::FK_ACTION_ID, DataType::INTEGER,
                      database::Model::Quest::DialogAction::TABLE,
                      database::Model::Quest::DialogAction::ID)
             );

    namespace ModelDialogGraph = database::Model::Quest::DialogGraph;
    db->query(Query::createQuery<Query::CREATE>(ModelDialogGraph::TABLE, db)
              .ifNotExists()
              .column(ModelDialogGraph::FK_BEFORE_ID, DataType::INTEGER,
                      ModelDialogLine::TABLE, ModelDialogLine::ID)
              .column(ModelDialogGraph::FK_AFTER_ID, DataType::INTEGER,
                      ModelDialogLine::TABLE, ModelDialogLine::ID)
              .constraint(ModelDialogGraph::FK_BEFORE_ID, Query::PRIMARY_KEY)
              .constraint(ModelDialogGraph::FK_AFTER_ID, Query::PRIMARY_KEY)
             );

    namespace ModelDialogAction = database::Model::Quest::DialogAction;
    db->query(Query::createQuery<Query::CREATE>(ModelDialogAction::TABLE, db)
              .ifNotExists()
              .column(ModelDialogAction::ID, DataType::INTEGER)
              .constraint(ModelDialogAction::ID, Query::PRIMARY_KEY)
              .constraint(ModelDialogAction::ID, Query::AUTOINCREMENT)
             );
    return verifyDatabaseModel(db);
}

}

