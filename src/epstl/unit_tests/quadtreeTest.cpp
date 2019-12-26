#include <quadtree.hpp>
#include <string>
#include <iostream>
#include "quadtreeTest.hpp"

TEST_F(quadtreeTest, Insertion)
{
    epstl::quadtree<float, int> tree(20, 20);
    EXPECT_EQ(tree.insert(5, 5, 100), 1);
    EXPECT_EQ(tree.depth(), 0);

    EXPECT_EQ(tree.insert(-5, -5, 10), 2);
    EXPECT_EQ(tree.depth(), 1);

    EXPECT_EQ(tree.insert(-5, 5, 20), 3);
    EXPECT_EQ(tree.depth(), 1);

    EXPECT_EQ(tree.insert(5, -5, 30), 4);
    EXPECT_EQ(tree.depth(), 1);

    EXPECT_EQ(tree.insert(2, 3, 300), 5);
    EXPECT_EQ(tree.depth(), 2);

    EXPECT_EQ(tree.insert(8, 3, 310), 6);
    EXPECT_EQ(tree.depth(), 2);

    EXPECT_EQ(tree.insert(2, 8, 320), 7);
    EXPECT_EQ(tree.depth(), 2);

    EXPECT_EQ(tree.insert(1, 2, 400), 8);
    EXPECT_EQ(tree.depth(), 3);

    EXPECT_EQ(tree.insert(0.5, 0.5, 410), 9);
    EXPECT_EQ(tree.depth(), 4);
}

TEST_F(quadtreeTest, InsertionCoherence)
{
    epstl::quadtree<int, int> tree(20, 20);
    EXPECT_EQ(tree.insert(5, 5, 100), 1);

    int value;
    ASSERT_TRUE(tree.get(5, 5, value));
    EXPECT_EQ(value, 100);

    EXPECT_EQ(tree.insert(3, 3, 110), 2);

    ASSERT_TRUE(tree.get(5, 5, value));
    EXPECT_EQ(value, 100);
    ASSERT_TRUE(tree.get(3, 3, value));
    EXPECT_EQ(value, 110);
}

TEST_F(quadtreeTest, GettingValue)
{
    epstl::quadtree<int, int> tree(20, 20);
    EXPECT_EQ(tree.insert(5, 5, 100), 1);
    EXPECT_EQ(*tree.at(5, 5), 100);

    int value;
    ASSERT_TRUE(tree.get(5, 5, value));
    EXPECT_EQ(value, 100);
}

TEST_F(quadtreeTest, ReplaceBehaviour)
{
    epstl::quadtree<int, int> tree(20, 20);
    EXPECT_EQ(tree.insert(5, 5, 100), 1);
    EXPECT_EQ(tree.insert(5, 5, 10), 1);
    EXPECT_EQ(*tree.at(5, 5), 10);

    tree.set_behaviour_flag(epstl::quadtree_no_replace);
    EXPECT_EQ(tree.insert(-5, 5, 100), 2);
    EXPECT_EQ(tree.insert(-5, 5, 10), 2);
    EXPECT_EQ(*tree.at(-5, 5), 100);
}
