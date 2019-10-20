#include "VectorTest.hpp"
#include <Vector.hpp>

namespace map
{

/*
 * Test the Vector Initializer List
 */
TEST_F(VectorTest, InitializerList)
{
    Vector<2> myVect{1, 2};
    EXPECT_EQ(myVect.at(0), 1);
    EXPECT_EQ(myVect.at(1), 2);
}

/*
 * Test class name
 */
TEST_F(VectorTest, ClassName)
{
    Vector<2> myVect;
    EXPECT_EQ(myVect.className(), "Vector");
}

/*
 * Test coordinate modification
 */
TEST_F(VectorTest, AtToModify)
{
    Vector<3> myVector{1, 2, 3};
    EXPECT_EQ(myVector.at(0), 1);
    EXPECT_EQ(myVector.at(1), 2);
    EXPECT_EQ(myVector.at(2), 3);

    myVector.at(1) = 5;
    EXPECT_EQ(myVector.at(1), 5);
}

/*
 * Test the implementation of x(), y() and z() shortcuts
 */
TEST_F(VectorTest, XYZImplementation)
{
    Vector<3> myVector{1, 2, 3};
    EXPECT_EQ(myVector.x(), 1);
    EXPECT_EQ(myVector.y(), 2);
    EXPECT_EQ(myVector.z(), 3);

    myVector.x() = 5;
    myVector.y() = 6;
    myVector.z() = 7;
    EXPECT_EQ(myVector.x(), 5);
    EXPECT_EQ(myVector.y(), 6);
    EXPECT_EQ(myVector.z(), 7);
}

/*
 * Norm test
 */
TEST_F(VectorTest, Norm)
{
    Vector<3> myVector{3, 4, 4};

    EXPECT_EQ(myVector.norm(), sqrt(3 * 3 + 4 * 4 + 4 * 4));
}

}


