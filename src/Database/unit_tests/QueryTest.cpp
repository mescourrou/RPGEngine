#include "QueryTest.hpp"
#include "Database.hpp"
namespace database {

/*
 * Test SELECT Query generation
 */
TEST_F(QueryTest, Select)
{
    std::string expected = "SELECT id, name FROM table1 WHERE id = 3;";

    auto query = Query::createQuery<Query::SELECT>("table1", database).column("id").column("name").where("id = 3");

    EXPECT_EQ(query.className(), "SelectQuery");
    EXPECT_EQ(query.str(), expected);

    EXPECT_EQ(database->columnsType("table1").size(), 2);

    expected = "SELECT id, name FROM table1 WHERE id = 2;";
    query = Query::createQuery<Query::SELECT>("table1", database).column("id").column("name").where("id", Query::EQUAL, "2");

    EXPECT_EQ(query.str(), expected);

    expected = "SELECT id, name FROM table1 WHERE name = 'Jack';";
    query = Query::createQuery<Query::SELECT>("table1", database).column("id").column("name").where("name", Query::EQUAL, "Jack");

    EXPECT_EQ(query.str(), expected);
}

/*
 * Test INSERT Query generation
 */
TEST_F(QueryTest, Insert)
{
    std::string expected = "";
    auto query = Query::createQuery<Query::INSERT>("aTable", database);
    EXPECT_EQ(query.className(), "InsertQuery");
    EXPECT_EQ(query.str(), expected);
    EXPECT_FALSE(query.isValid());

    expected = "INSERT INTO aTable (first_name, last_name) VALUES ('Solomon', 'Lane');";
    EXPECT_EQ(Query::createQuery<Query::INSERT>("aTable", database)
              .value("first_name", "Solomon")
              .value("last_name", "Lane").str(), expected);
}

/*
 * Test CREATE Query generation
 */
TEST_F(QueryTest, Create)
{
    std::string expected = "";
    auto query = Query::createQuery<Query::CREATE>("newTable", database);
    EXPECT_EQ(query.className(), "CreateQuery");
    EXPECT_EQ(query.str(), expected);
    EXPECT_FALSE(query.isValid());

    query = Query::createQuery<Query::CREATE>("newTable", database).contraint("PRIMARY KEY (first_name, last_name)");
    EXPECT_EQ(query.str(), expected);
    EXPECT_FALSE(query.isValid());

    expected = "CREATE TABLE newTable (first_name BLOB, last_name BLOB, age INTEGER PRIMARY KEY (first_name, last_name));";
    query = Query::createQuery<Query::CREATE>("newTable", database)
            .column("first_name").column("last_name").column("age", INTEGER)
            .contraint("PRIMARY KEY (first_name, last_name)");

    EXPECT_EQ(query.str(), expected);
    EXPECT_TRUE(query.isValid());

}

/*
 * Test UPDATE Query generation
 */
TEST_F(QueryTest, Update)
{
    std::string expected = "";
    auto query = Query::createQuery<Query::UPDATE>("table1", database);

    EXPECT_EQ(query.className(), "UpdateQuery");
    EXPECT_EQ(query.str(), expected);
    EXPECT_FALSE(query.isValid());

    query = Query::createQuery<Query::UPDATE>("table1", database).where("i = 1");
    EXPECT_EQ(query.str(), expected);
    EXPECT_FALSE(query.isValid());

    expected = "UPDATE table1 SET i = 1, j = 3 WHERE k = 3;";
    query = Query::createQuery<Query::UPDATE>("table1", database)
            .set("i", "1").set("j", "3")
            .where("k = 3");
    EXPECT_EQ(query.str(), expected);
    EXPECT_TRUE(query.isValid());
}

void QueryTest::SetUp()
{

    database.reset(new Database("data/db_sample2.db"));
}

}


