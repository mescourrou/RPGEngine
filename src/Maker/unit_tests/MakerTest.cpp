#include "MakerTest.hpp"
#include <glog/logging.h>

#include <Database.hpp>

#include <filesystem>

namespace maker {

/*
 * Test the loading of a non existant database
 */
TEST_F(MakerTest, LoadingDatabaseOpenning)
{
    std::string dbFilename = "not_existing_directory/sample0.db";
    Maker maker;

    Maker::DatabaseStatus expectedStatus = Maker::NOT_LOADED;
    Maker::DatabaseStatus actualStatus;

    auto cb = [&](Maker::DatabaseStatus status){ actualStatus = status;};

    maker.subscribeDatabaseLoaded(cb);

    maker.loadDatabase(dbFilename);
    usleep(100);

    EXPECT_EQ(actualStatus, expectedStatus);
}

/*
 * Test the loading of an empty database
 */
TEST_F(MakerTest, LoadingDatabaseEmpty)
{
    std::filesystem::path usedFile = "data/sample0.db";

    std::filesystem::remove(usedFile);
    Maker maker;

    Maker::DatabaseStatus expectedStatus = Maker::EMPTY;
    Maker::DatabaseStatus actualStatus;

    auto cb = [&](Maker::DatabaseStatus status){ actualStatus = status;};

    maker.subscribeDatabaseLoaded(cb);

    maker.loadDatabase(usedFile);
    usleep(100);

    EXPECT_EQ(actualStatus, expectedStatus);
}

/*
 * Test the creation of the database
 */
TEST_F(MakerTest, CreatingDatabaseModel)
{
    std::filesystem::path usedFile = "data/sample0.db";

    std::filesystem::remove(usedFile);
    Maker maker;

    Maker::DatabaseStatus actualStatus = Maker::NOT_LOADED;

    auto cb = [&](Maker::DatabaseStatus status){ actualStatus = status;};

    maker.subscribeDatabaseLoaded(cb);
    maker.loadDatabase(usedFile);
    usleep(100);

    ASSERT_NE(actualStatus, Maker::NOT_LOADED);

    EXPECT_TRUE(maker.createDatabaseModel());
}

}

int main(int argc, char **argv)
{
    ::google::InitGoogleLogging(argv[0]);
    ::google::LogToStderr();
    ::google::SetVLOGLevel("*", VERBOSE);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

