#include "DatabaseTest.hpp"
namespace database {

TEST_F(DatabaseTest, QueryText)
{
    EXPECT_TRUE(m_database->query("SELECT * FROM table1 WHERE id = 1"));
    ASSERT_EQ(m_database->m_result->size(), 3);

    EXPECT_EQ(m_database->m_result->at(0).at("status"), "success");

    EXPECT_EQ(m_database->m_result->at(1).at("id"), "1");
    EXPECT_EQ(m_database->m_result->at(1).at("name"), "Good morning");

    EXPECT_EQ(m_database->m_result->at(2).at("id"), "1");
    EXPECT_EQ(m_database->m_result->at(2).at("name"), "Good evening");

}

TEST_F(DatabaseTest, QuerySelect)
{
    auto result = m_database->query(Query::createQuery<Query::SELECT>("table1")
                                    .where("id = 2"));

    ASSERT_EQ(result.size(), 2);
    EXPECT_EQ(result.at(0).at("status"), "success");

    EXPECT_EQ(result.at(1).at("id"), "2");
    EXPECT_EQ(result.at(1).at("name"), "Hello World");

    result = m_database->query(Query::createQuery<Query::SELECT>("table1")
                                        .where("id = 1").where("name = 'Good morning'"));

    ASSERT_EQ(result.size(), 2);
    EXPECT_EQ(result.at(0).at("status"), "success");

    EXPECT_EQ(result.at(1).at("id"), "1");
    EXPECT_EQ(result.at(1).at("name"), "Good morning");
}

TEST_F(DatabaseTest, Insert)
{
    auto result = m_database->query(Query::createQuery<Query::INSERT>("table1")
                                    .value("id", "3").value("name", "'Hello guys'"));
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result.at(0).at("status"), "success");

    result = m_database->query(Query::createQuery<Query::SELECT>("table1")
                               .where("id = 3"));
    ASSERT_EQ(result.size(), 2);
    EXPECT_EQ(result.at(0).at("status"), "success");
    EXPECT_EQ(result.at(1).at("id"), "3");
    EXPECT_EQ(result.at(1).at("name"), "Hello guys");
}

TEST_F(DatabaseTest, Update)
{
    auto result = m_database->query(Query::createQuery<Query::UPDATE>("table1")
                                    .set("id", "4").where("name = 'Good evening'"));
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result.at(0).at("status"), "success");

    result = m_database->query(Query::createQuery<Query::SELECT>("table1")
                               .where("id = 4"));
    ASSERT_EQ(result.size(), 2);
    EXPECT_EQ(result.at(0).at("status"), "success");
    EXPECT_EQ(result.at(1).at("id"), "4");
    EXPECT_EQ(result.at(1).at("name"), "Good evening");
}

void DatabaseTest::SetUp()
{
    std::filesystem::path usedFile = "data/db_sample1.db";
    std::filesystem::path modelFile = "data/db_sample1.db.in";
    if (std::filesystem::exists(usedFile))
        std::filesystem::remove(usedFile);
    if (!std::filesystem::exists(modelFile))
        LOG(FATAL) << "File " << modelFile << " doen't exists";
    std::filesystem::copy(modelFile, usedFile);
    m_database.reset(new Database(usedFile));
}

}


int main(int argc, char **argv)
{
    google::InitGoogleLogging(argv[0]);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

