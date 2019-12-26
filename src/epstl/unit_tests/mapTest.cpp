#include <map.hpp>
#include "mapTest.hpp"

namespace epstl
{

TEST_F(mapTest, insert)
{
    map<int, int> m;

    EXPECT_TRUE(m.insert(10, 0));
    EXPECT_EQ(*m.at(10), 0);

    EXPECT_FALSE(m.insert(10, 1));

    EXPECT_TRUE(m.insert(13, 2));
    EXPECT_TRUE(m.insert(12, 3));
}

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

} // namespace epstl
