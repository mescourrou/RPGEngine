#include "QueryTest.hpp"
namespace database {

TEST_F(QueryTest, Select)
{
    std::string expected = "SELECT id, name FROM aTable WHERE id = 3;";

    auto query = Query::createQuery<Query::SELECT>("aTable").column("id").column("name").where("id = 3");

    EXPECT_EQ(query.className(), "SelectQuery");
    EXPECT_EQ(query.str(), expected);
}

TEST_F(QueryTest, Insert)
{
    std::string expected = "";
    auto query = Query::createQuery<Query::INSERT>("aTable");
    EXPECT_EQ(query.className(), "InsertQuery");
    EXPECT_EQ(query.str(), expected);
    EXPECT_FALSE(query.isValid());

    expected = "INSERT INTO aTable (first_name, last_name) VALUES (Solomon, Lane);";
    EXPECT_EQ(Query::createQuery<Query::INSERT>("aTable")
              .value("first_name", "Solomon")
              .value("last_name", "Lane").str(), expected);
}

TEST_F(QueryTest, Create)
{
    std::string expected = "";
    auto query = Query::createQuery<Query::CREATE>("newTable");
    EXPECT_EQ(query.className(), "CreateQuery");
    EXPECT_EQ(query.str(), expected);
    EXPECT_FALSE(query.isValid());

    query = Query::createQuery<Query::CREATE>("newTable").contraint("PRIMARY KEY (first_name, last_name)");
    EXPECT_EQ(query.str(), expected);
    EXPECT_FALSE(query.isValid());

    expected = "CREATE TABLE newTable (first_name, last_name, age INTEGER PRIMARY KEY (first_name, last_name));";
    query = Query::createQuery<Query::CREATE>("newTable")
            .column("first_name").column("last_name").column("age", "INTEGER")
            .contraint("PRIMARY KEY (first_name, last_name)");

    EXPECT_EQ(query.str(), expected);
    EXPECT_TRUE(query.isValid());

}

TEST_F(QueryTest, Update)
{
    std::string expected = "";
    auto query = Query::createQuery<Query::UPDATE>("table1");

    EXPECT_EQ(query.className(), "UpdateQuery");
    EXPECT_EQ(query.str(), expected);
    EXPECT_FALSE(query.isValid());

    query = Query::createQuery<Query::UPDATE>("table1").where("i = 1");
    EXPECT_EQ(query.str(), expected);
    EXPECT_FALSE(query.isValid());

    expected = "UPDATE table1 SET i = 1, j = 3 WHERE k = 3;";
    query = Query::createQuery<Query::UPDATE>("table1")
            .set("i", "1").set("j", "3")
            .where("k = 3");
    EXPECT_EQ(query.str(), expected);
    EXPECT_TRUE(query.isValid());
}

}


