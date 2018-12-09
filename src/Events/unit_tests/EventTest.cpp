#include "EventTest.hpp"
namespace events {

TEST_F(EventTest, StandardFunction)
{
    Event<void> e1;
    bool noArgCb = false;
    auto cb = [&noArgCb] () {
        noArgCb = true;
    };

    e1.subscribe(cb);

    e1.trigger();

    Event<int> e2;
    bool argCb = false;
    auto cb2 = [&argCb] (int number) {
        if (number == 3)
            argCb = true;
    };

    e2.subscribe(cb2);

    e2.trigger(3);

    usleep(10000);

    EXPECT_TRUE(noArgCb);
    EXPECT_TRUE(argCb);

}

class AClass : public BaseObject
{
public:
    AClass() = default;
    ~AClass() override = default;

    std::string className() const noexcept override { return "AClass"; }
    void noArgMethod() { m_noArgMethod = true; }
    void argMethod(std::string word) { if (word == "Hello") m_noArgMethod = true; }

    bool m_noArgMethod = false;
};

TEST_F(EventTest, MethodFunction)
{
    Event<void> e;
    Event<std::string> e2;
    AClass o;

    e.subscribe(std::bind(&AClass::noArgMethod, &o));

    e.trigger();

    e2.subscribe(std::bind(&AClass::argMethod, &o, std::placeholders::_1));

    e2.trigger("Hello");

    usleep(1000);

    EXPECT_TRUE(o.m_noArgMethod);

}

}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

