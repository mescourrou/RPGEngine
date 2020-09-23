#include "Object.hpp"

// Project
#include <Model.hpp>
#include <Database.hpp>
#include <Query.hpp>
#include <VerbosityLevels.hpp>

// External lib
#include <glog/logging.h>

namespace object
{

/**
 * @brief Construct an object
 * @param name Name of the object
 */
Object::Object(std::string name) : m_name(std::move(name))
{
    VLOG(verbosityLevel::OBJECT_CREATION) << "Creating " << className() << " => " <<
                                          this;
}

/**
 * @brief Load the object from the database
 * @param [in] db Database to use
 * @return Return true if the loading was successfull
 */
bool Object::loadFromDatabase(std::shared_ptr<databaseTools::Database> db)
{
    PROFILE_FUNCTION();
    namespace Model = database::Model::Object;
    using namespace databaseTools;
    if (!db)
        throw ObjectException("No database given.",
                              BaseException::MISSING_DATABASE);
    if (!verifyDatabaseModel(db))
        throw ObjectException("The database model is not correct",
                              BaseException::BAD_MODEL);

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
std::shared_ptr<Object> Object::createFromDatabase(const std::string& name,
        std::shared_ptr<databaseTools::Database> db)
{
    PROFILE_FUNCTION();
    namespace Model = database::Model::Object;
    using namespace databaseTools;
    if (!db)
        throw ObjectException("No database given.",
                              BaseException::MISSING_DATABASE);
    if (!verifyDatabaseModel(db))
        throw ObjectException("The database model is not correct",
                              BaseException::BAD_MODEL);

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
        throw ObjectException(std::string("Object type '").append(
                                  objectType).append("' unknown"), ObjectException::UNKNOWN_OBJECT_TYPE);

    return ret;
}

/**
 * @brief Verify if the database contains all the information needed
 * @param [in] db Database to verify
 * @return Return true if the database if valid
 */
bool Object::verifyDatabaseModel(std::shared_ptr<databaseTools::Database> db)
{
    PROFILE_FUNCTION();
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
 * @brief Create the table needed in the database
 * @param db Database to populate
 * @return Return true if the database was well populated
 */
bool Object::createDatabaseModel(std::shared_ptr<databaseTools::Database> db)
{
    PROFILE_FUNCTION();
    namespace Model = database::Model::Object;
    using namespace databaseTools;

    if (!db)
        throw ObjectException("No database given.",
                              BaseException::MISSING_DATABASE);

    db->query(Query::createQuery<Query::CREATE>(Model::TABLE, db).ifNotExists()
              .column(Model::NAME, DataType::BLOB).constraint(Model::NAME, Query::PRIMARY_KEY)
              .column(Model::TYPE, DataType::BLOB)
              .column(Model::VALUE, DataType::INTEGER));

    return verifyDatabaseModel(db);
}

} // namespace object

/**
 * @brief Print the object into the stream
 * @param stream Stream into write on
 * @param object Object to print
 * @return Modified stream
 */
std::ostream& object::operator<<(std::ostream& stream, const Object& object)
{
    stream << "Object : " << object.name() << std::endl;
    stream << object.description() << std::endl;

    return stream;
}
