#include <map.hpp>
#include "mapTest.hpp"

namespace epstl
{

/*
 * Insert values in the map
 */
TEST_F(mapTest, insert)
{
    map<int, int> m;

    EXPECT_TRUE(m.insert(10, 0));
    EXPECT_EQ(*m.at(10), 0);

    EXPECT_FALSE(m.insert(10, 1));

    EXPECT_TRUE(m.insert(13, 2));
    EXPECT_TRUE(m.insert(12, 3));

    EXPECT_EQ(m.size(), 3);
}

/*
 * Test the rotations
 */
TEST_F(mapTest, rotation)
{
    map<char, int> m;
    m.insert('A', 1);
    m.insert('B', 2);
    m.insert('C', 3);
    m.insert('D', 4);
    m.insert('E', 5);
    m.insert('F', 6);

}

/*
 * Erase keys of the map
 */
TEST_F(mapTest, erase)
{
    map<int, int> m;

    m.insert(10, 1);
    m.insert(13, 2);
    m.insert(12, 3);
    m.insert(8, 4);

    EXPECT_EQ(m.size(), 4);
    EXPECT_EQ(m.height(), 3);

    EXPECT_EQ(m.erase(13), 3);
    // Not existant node
    EXPECT_EQ(m.erase(11), 3);
    // Already erased node
    EXPECT_EQ(m.erase(13), 3);

    EXPECT_EQ(m.erase(12), 2);
    EXPECT_EQ(m.height(), 2);

    EXPECT_EQ(m.erase(8), 1);
}

TEST_F(mapTest, iterator)
{
    map<int, int> m;

    m.insert(10, 1);
    m.insert(13, 2);
    m.insert(12, 3);
    m.insert(8, 4);

    std::vector<int> item_expectations{4, 1, 3, 2};
    std::vector<int> key_expectation{8, 10, 12, 13};

    short i = 0;
    for (auto& item : m)
    {
        EXPECT_EQ(item.second, item_expectations.at(i));
        i++;
    }
    i = 0;
    for (auto& [k, item] : m)
    {
        EXPECT_EQ(k, key_expectation.at(i));
        EXPECT_EQ(item, item_expectations.at(i));
        i++;
    }
}

} // namespace epstl