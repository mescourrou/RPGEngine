#include "Object.hpp"
#include <Model.hpp>
#include <Database.hpp>
#include <Query.hpp>

/**
 * @brief Construct an object
 * @param name Name of the object
 */
object::Object::Object(std::string name) : m_name(std::move(name))
{

}

/**
 * @brief Load the object from the database
 * @param [in] db Database to use
 * @return Return true if the loading was successfull
 */
bool object::Object::loadFromDatabase(std::shared_ptr<database::Database> db)
{
    namespace Model = database::Model::Object;
    using namespace database;
    if (!db)
        throw ObjectException("No database given.", Database::DatabaseException::MISSING_DATABASE);
    if (!verifyDatabaseModel(db))
        throw ObjectException("The database model is not correct", Database::DatabaseException::BAD_MODEL);

    auto result = db->query(Query::createQuery<Query::SELECT>(Model::TABLE, db)
                            .where(Model::NAME, Query::EQUAL, m_name));
    if (result.size() <= 1)
        return false;
    m_value = Money{static_cast<unsigned int>(std::atoi(result.at(1).at(Model::VALUE).c_str()))};
    return true;
}

/**
 * @brief Create an object from the database, of the good type
 * @param [in] name Name of the object (same than in the database)
 * @param [in] db Database to use
 * @return Return a object, or a derived type
 */
std::shared_ptr<object::Object> object::Object::createFromDatabase(const std::string &name, std::shared_ptr<database::Database> db)
{
    namespace Model = database::Model::Object;
    using namespace database;
    if (!db)
        throw ObjectException("No database given.", Database::DatabaseException::MISSING_DATABASE);
    if (!verifyDatabaseModel(db))
        throw ObjectException("The database model is not correct", Database::DatabaseException::BAD_MODEL);

    auto result = db->query(Query::createQuery<Query::SELECT>(Model::TABLE, db)
                                      .column(Model::TYPE)
                                      .where(Model::NAME, Query::EQUAL, name));
    if (result.size() <= 1)
        return {};
    std::string objectType = result.at(1).at(Model::TYPE);
    std::shared_ptr<Object> ret;
    if (objectType == Model::ObjectType::OBJECT)
    {
        ret = std::shared_ptr<Object>(new Object(name));
        ret->loadFromDatabase(db);
    }
    else
        throw ObjectException(std::string("Object type '").append(objectType).append("' unknown"), ObjectException::UNKNOWN_OBJECT_TYPE);

    return ret;
}

/**
 * @brief Verify if the database contains all the information needed
 * @param [in] db Database to verify
 * @return Return true if the database if valid
 */
bool object::Object::verifyDatabaseModel(std::shared_ptr<database::Database> db)
{
    namespace Model = database::Model::Object;
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
        else if (column == Model::VALUE)
                goodColumns++;
        else
            return false;
    }
    if (goodColumns != 3)
        return false;

    return true;
}


/**
 * @brief Print the object into the stream
 * @param stream Stream into write on
 * @param object Object to print
 * @return Modified stream
 */
std::ostream &object::operator<<(std::ostream &stream, const Object& object)
{
    stream << "Object : " << object.name() << std::endl;
    stream << object.description() << std::endl;

    return stream;
}

