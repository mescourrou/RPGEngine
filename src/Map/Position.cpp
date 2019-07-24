#include "Position.hpp"

// Project
#include <Map.hpp>
#include <Model.hpp>
#include <Query.hpp>
#include <Database.hpp>

// Std lib
#include <string>

namespace map {

/**
 * @brief Construct a position from the given map and coordinates
 * @param map Map
 * @param x Coordinate x
 * @param y Coordinate y
 * @param z Coordinate z
 */
Position::Position(std::shared_ptr<Map> map, double x, double y, double z) :
    m_map(map), m_position{x,y,z}
{
}

/**
 * @brief Load the character position from the database
 * @param db Database to use
 * @param context Context to use to create the map
 * @param characterName Character from whom loading the position
 * @return Return true if all went well
 */
bool Position::loadFromDatabase(std::shared_ptr<database::Database> db, std::shared_ptr<config::Context> context, const std::string &characterName)
{
    namespace Model = database::Model::Position;
    using namespace database;
    if (!db)
        throw PositionException("No database given.", DatabaseException::MISSING_DATABASE);
    if (!verifyDatabaseModel(db))
        throw PositionException("The database model is not correct", DatabaseException::BAD_MODEL);

    auto result = db->query(Query::createQuery<Query::SELECT>(Model::TABLE, db).where(Model::FK_CHARACTER, Query::EQUAL, characterName));
    if (!Database::isQuerySuccessfull(result))
        return false;
    if (result.size() <= 1) // No result
        return false; // Stay with the previous configuration

    m_position.x() = std::stod(result.at(1).at(Model::X));
    m_position.y() = std::stod(result.at(1).at(Model::Y));
    m_position.z() = std::stod(result.at(1).at(Model::Z));

    if (result.at(1).at(Model::FK_MAP) == "NULL")
        return false;
    m_map = std::make_shared<map::Map>(context, result.at(1).at(Model::FK_MAP));

    return true;

}

/**
 * @brief Create the position with the given coordinates
 * @param x Coordinate x
 * @param y Coordinate y
 * @param z Coordinate z
 */
Position::Position(double x, double y, double z) :
    m_position{x,y,z}
{
}

bool Position::operator==(const Position &cmp) const
{
    return (m_map && cmp.m_map && *m_map == *cmp.m_map) &&
            m_position == cmp.m_position;
}

bool Position::operator!=(const Position &cmp) const
{
    return (m_map && cmp.m_map && *m_map != *cmp.m_map) ||
            m_position != cmp.m_position;
}

/**
 * @brief Move the position
 * @param move Vector to follow
 */
void Position::move(const Vector<3> &move)
{
    m_position += move;
}

/**
 * @brief Calculate the distance to the other position
 */
double Position::distanceTo(const Position &other) const
{
    if (m_map && other.m_map)
    {
        if (m_map->name() != other.m_map->name())
            throw PositionException("Position not in the same map");
    }
    return Vector<3>(other.m_position - m_position).norm();
}

bool Position::operator==(const Position &other)
{
    bool same = true;
    if (m_map && other.m_map)
    {
        if (*m_map != *other.m_map)
            same = false;
    }
    return m_position == other.m_position && same;
}

bool Position::operator!=(const Position &other)
{
    bool diff = false;
    if (m_map && other.m_map)
    {
        if (*m_map != *other.m_map)
            diff = true;
    }
    return m_position != other.m_position || diff;
}

/**
 * @brief Verify the database model
 * @param db Database to verify
 * @return Return true if the model is good
 */
bool Position::verifyDatabaseModel(std::shared_ptr<database::Database> db)
{
    namespace Model = database::Model::Position;
    using namespace database;
    if (!db->isTable(Model::TABLE))
        return false;
    auto columnList = db->columnList(Model::TABLE);

    unsigned short goodColumns = 0;
    for (auto& column : columnList)
    {
        if (column == Model::FK_CHARACTER)
            goodColumns++;
        else if (column == Model::FK_MAP)
            goodColumns++;
        else if (column == Model::X)
            goodColumns++;
        else if (column == Model::Y)
            goodColumns++;
        else if (column == Model::Z)
            goodColumns++;
        else
            return false;
    }

    if (goodColumns == 5)
        return true;
    return false;
}

/**
 * @brief Create the table needed
 * @param db Database to populate
 * @return Return true if all went well
 */
bool Position::createDatabaseModel(std::shared_ptr<database::Database> db)
{
    namespace Model = database::Model::Position;
    using namespace database;
    if (!db)
        throw PositionException("No database given.", DatabaseException::MISSING_DATABASE);

    db->query(Query::createQuery<Query::CREATE>(Model::TABLE, db).ifNotExists()
              .column(Model::FK_CHARACTER, DataType::BLOB, database::Model::Character::TABLE, database::Model::Character::NAME)
              .constraint(Model::FK_CHARACTER, Query::PRIMARY_KEY)
              .column(Model::X, DataType::INTEGER)
              .column(Model::Y, DataType::INTEGER)
              .column(Model::Z, DataType::INTEGER)
              .column(Model::FK_MAP, DataType::BLOB));

    return verifyDatabaseModel(db);

}

} // namespace map
