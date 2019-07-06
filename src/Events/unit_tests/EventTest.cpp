#include "EventTest.hpp"
namespace events {

/*
 * Test the subscribe and trigger for standard function (no object)
 */
TEST_F(EventTest, StandardFunction)
{
    Event<void> e1;
    bool noArgCb = false;
    auto cb = [&noArgCb] () {
        noArgCb = true;
    };

    e1.subscribeAsync(cb);

    e1.trigger();

    Event<int> e2;
    bool argCb = false;
    auto cb2 = [&argCb] (int number) {
        if (number == 3)
            argCb = true;
    };

    e2.subscribeAsync(cb2);

    e2.trigger(3);

    usleep(50000);

    EXPECT_TRUE(noArgCb);
    EXPECT_TRUE(argCb);

}

class AClass : public BaseObject
{
    DECLARE_BASEOBJECT(AClass)
public:
    AClass() = default;
    ~AClass() override = default;

    void noArgMethod() { m_noArgMethod = true; }
    void argMethod(std::string word) { if (word == "Hello") m_noArgMethod = true; }

    bool m_noArgMethod = false;
};

/*
 * Test subscribe and trigger for objet's methods
 */
TEST_F(EventTest, MethodFunction)
{
    Event<void> e;
    Event<std::string> e2;
    AClass o;

    e.subscribeSync(&o, &AClass::noArgMethod);

    e.trigger();

    e2.subscribeSync(&o, &AClass::argMethod);

    e2.trigger("Hello");

    usleep(50000);

    EXPECT_TRUE(o.m_noArgMethod);

}

}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

