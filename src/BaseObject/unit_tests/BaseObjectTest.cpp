#include "BaseObjectTest.hpp"

/**
 * @brief Implementation of abstract class AnObject to test
 */
class AnObject : public BaseObject
{
    DECLARE_BASEOBJECT(AnObject)
    friend class BaseObjectTest;
public:
    AnObject(BaseObject* parent = nullptr) : BaseObject(parent) {}
    ~AnObject() override = default;
};

/*
 * Test the default behaviour when no parent is given
 */
TEST_F(BaseObjectTest, NullParent)
{
    AnObject mySon;

    EXPECT_EQ(mySon.parent(), nullptr);
}

/*
 * Test in-constructor parent setting
 */
TEST_F(BaseObjectTest, SetParentAtTheBeginning)
{
    AnObject parent;

    AnObject mySon(&parent);

    EXPECT_EQ(mySon.parent(), &parent);
}

/*
 * Test post creation assigning parent
 */
TEST_F(BaseObjectTest, SetParentAfter)
{
    AnObject parent;

    AnObject mySon;
    EXPECT_EQ(mySon.parent(), nullptr);

    mySon.setParent(&parent);
    EXPECT_EQ(mySon.parent(), &parent);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

