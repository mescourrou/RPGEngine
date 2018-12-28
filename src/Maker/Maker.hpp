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
class MakerException : public BaseException
{
public:
    static const inline Errors VERSION = Errors(__COUNTER__);
    MakerException(const std::string& w, const Errors& code = BaseException::UNKNOWN):
        BaseException(w, code) {}
};

#ifdef RPG_BUILD_TEST
class MakerTest;
#endif

class Maker : public BaseObject
{
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

	std::string className() const noexcept override { return "Maker"; }

    bool createDatabaseModel();
private:
    static bool verifyDatabaseModel(std::shared_ptr<database::Database> db);

    std::shared_ptr<database::Database> m_db;

    events::Event<DatabaseStatus> m_signalDatabaseLoaded;
};

} // namespace maker

