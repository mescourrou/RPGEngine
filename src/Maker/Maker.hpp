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

class Maker : public BaseObject
{
    DECLARE_BASEOBJECT(Maker)
#ifdef RPG_BUILD_TEST
	friend class maker::MakerTest;
#endif
public:
    enum DatabaseStatus {
        NOT_LOADED,
        EMPTY,
        NOT_VALID,
        VALID
    };
    Maker();
    ~Maker() override = default;

    void loadDatabase(const std::string& filename);
    void subscribeDatabaseLoaded(std::function<void(DatabaseStatus)> func) { m_signalDatabaseLoaded.subscribeAsync(func);}

    bool createDatabaseModel();
private:
    static bool verifyDatabaseModel(std::shared_ptr<database::Database> db);

    std::shared_ptr<database::Database> m_db;

    events::Event<DatabaseStatus> m_signalDatabaseLoaded;
};

} // namespace maker

