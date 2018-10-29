#include "BaseObjectTest.hpp"

class AnObject : public BaseObject
{
    friend class BaseObjectTest;
public:
    AnObject(BaseObject* parent = nullptr) : BaseObject(parent) {}
    ~AnObject() override = default;
    std::string className() const noexcept override { return "AnObject"; }
};

TEST_F(BaseObjectTest, NullParent)
{
    AnObject mySon;

    EXPECT_EQ(mySon.parent(), nullptr);
}

TEST_F(BaseObjectTest, SetParentAtTheBeginning)
{
    // Init
    AnObject parent;

    // Scenario
    AnObject mySon(&parent);

    EXPECT_EQ(mySon.parent(), &parent);
}

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

