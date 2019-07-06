#pragma once

// Project
#include "general_config.hpp"
#include <BaseObject.hpp>
#include <Event.hpp>
#include <BaseException.hpp>

#ifdef RPG_BUILD_TEST
#include <gtest/gtest.h>
#endif

namespace database {
class Database;
}

namespace maker
{
/**
 * @brief Manage the exceptions of Game
 */
CREATE_EXCEPTION_CLASS(Maker,
                       ADD_EXCEPTION_CODE(VERSION))

#ifdef RPG_BUILD_TEST
class MakerTest;
#endif

/**
 * @brief Class to manage the game maker
 */
class Maker : public BaseObject
{
    DECLARE_BASEOBJECT(Maker)
#ifdef RPG_BUILD_TEST
	friend class maker::MakerTest;
#endif
public:
    /**
     * @brief Database status
     */
    enum DatabaseStatus {
        NOT_LOADED, ///< Database not loaded
        EMPTY, ///< Database empty
        NOT_VALID, ///< Database not valid
        VALID ///< Valid database model
    };
    /// @brief Constructor
    Maker() = default;
    /// @brief Destructor
    ~Maker() override = default;

    void loadDatabase(const std::string& filename);

    /**
     * @brief Subscribe to the signal about the status of the database load
     * @param func Function to subscribe
     */
    void subscribeDatabaseLoaded(std::function<void(DatabaseStatus)> func)
    { m_signalDatabaseLoaded.subscribeAsync(func);}

    bool createDatabaseModel();
private:
    static bool verifyDatabaseModel(std::shared_ptr<database::Database> db);

    std::shared_ptr<database::Database> m_db; ///< Database to use

    events::Event<DatabaseStatus> m_signalDatabaseLoaded; ///< Signal once the database is loaded
};

} // namespace maker

