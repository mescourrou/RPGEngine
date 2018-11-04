#include "MoneyTest.hpp"
namespace object {

using MoneyDeathTest = MoneyTest;

/*
 * Test behaviour when the money system is not initialized
 */
TEST_F(MoneyTest, NotInitialized)
{
    EXPECT_THROW(new Money(), std::string);
}

/*
 * Test initialization
 */
TEST_F(MoneyTest, FinancialSystemInit)
{
    auto names = Money::moneyNames();
    ASSERT_EQ(names.size(), 3);
    EXPECT_EQ(names.at(0), "bronze");
    EXPECT_EQ(names.at(1), "argent");
    EXPECT_EQ(names.at(2), "or");

    EXPECT_EQ(Money::moneyValue("nonValidMoneyName"), 0);
    EXPECT_EQ(Money::moneyValue("bronze"), 1);
    EXPECT_EQ(Money::moneyValue("argent"), 100);
    EXPECT_EQ(Money::moneyValue("or"), 50000);
}

/*
 * Test class name
 */
TEST_F(MoneyTest, ClassName)
{
    Money money;
    EXPECT_EQ(money.className(), "Money");
}

/*
 * Test if money constructor initialize at 0
 */
TEST_F(MoneyTest, NoMoney)
{
    Money money;
    EXPECT_EQ(money.value("bronze"), 0);
    EXPECT_EQ(money.value("argent"), 0);
    EXPECT_EQ(money.value("or"), 0);

}

/*
 * Add test
 */
TEST_F(MoneyTest, Add)
{
    Money money;

    // Simple add

    money.add("bronze", 50);
    EXPECT_EQ(money.value("bronze"), 50);
    EXPECT_EQ(money.value("argent"), 0);
    EXPECT_EQ(money.value("or"), 0);

    money.add("argent", 480);
    EXPECT_EQ(money.value("bronze"), 50);
    EXPECT_EQ(money.value("argent"), 480);
    EXPECT_EQ(money.value("or"), 0);

    money.add("or", 5);
    EXPECT_EQ(money.value("bronze"), 50);
    EXPECT_EQ(money.value("argent"), 480);
    EXPECT_EQ(money.value("or"), 5);


    // With Carry

    money.add("bronze", 60);
    EXPECT_EQ(money.value("bronze"), 10);
    EXPECT_EQ(money.value("argent"), 481);
    EXPECT_EQ(money.value("or"), 5);

    money.add("argent", 30);
    EXPECT_EQ(money.value("bronze"), 10);
    EXPECT_EQ(money.value("argent"), 11);
    EXPECT_EQ(money.value("or"), 6);


    // With huge numbers

    money.add("bronze", 6005);
    EXPECT_EQ(money.value("bronze"), 15);
    EXPECT_EQ(money.value("argent"), 71);
    EXPECT_EQ(money.value("or"), 6);

    money.add("bronze", 600005);
    EXPECT_EQ(money.value("bronze"), 20);
    EXPECT_EQ(money.value("argent"), 71);
    EXPECT_EQ(money.value("or"), 18);

}

/*
 * Testing the initializer list
 */
TEST_F(MoneyTest, InitializerList)
{
    Money money1{10};
    EXPECT_EQ(money1.value("bronze"), 10);
    EXPECT_EQ(money1.value("argent"), 0);
    EXPECT_EQ(money1.value("or"), 0);

    Money money2{10, 40};
    EXPECT_EQ(money2.value("bronze"), 10);
    EXPECT_EQ(money2.value("argent"), 40);
    EXPECT_EQ(money2.value("or"), 0);

    Money money3{10, 40, 50};
    EXPECT_EQ(money3.value("bronze"), 10);
    EXPECT_EQ(money3.value("argent"), 40);
    EXPECT_EQ(money3.value("or"), 50);

    // With Carry
    Money money4{140, 40, 50};
    EXPECT_EQ(money4.value("bronze"), 40);
    EXPECT_EQ(money4.value("argent"), 41);
    EXPECT_EQ(money4.value("or"), 50);

}

/*
 * Method we expect to kill the process
 */
void MoneyTest::_createInvalidInitializeList()
{
    new Money{10, 10, 10, 10};
}

/*
 * Test if the initializer list manage invalid number of arguments
 */
TEST_F(MoneyDeathTest, InvalidInitializerList)
{
    EXPECT_DEATH(_createInvalidInitializeList(), "");
}

/*
 * Test the conversion into base value
 */
TEST_F(MoneyTest, ConvertToBaseValue)
{
    Money money{10, 40, 50};
    EXPECT_EQ(money.convertToBaseMoney(), 2504010);
}

/*
 * Test logical operators : ==, !=, >=, >, <=, <
 */
TEST_F(MoneyTest, LogicalOperators)
{
    Money reference{10, 20, 30};
    Money same{10, 20, 30};
    Money less{40, 30, 10};
    Money more{9, 3, 40};

    EXPECT_TRUE(reference == same);
    EXPECT_FALSE(reference == less);
    EXPECT_FALSE(reference == more);

    EXPECT_TRUE(reference != less);
    EXPECT_TRUE(reference != more);
    EXPECT_FALSE(reference != same);

    EXPECT_TRUE(reference >= same);
    EXPECT_TRUE(reference >= less);
    EXPECT_FALSE(reference >= more);

    EXPECT_TRUE(reference <= same);
    EXPECT_TRUE(reference <= more);
    EXPECT_FALSE(reference <= less);

    EXPECT_TRUE(reference < more);
    EXPECT_FALSE(reference < same);
    EXPECT_FALSE(reference < less);

    EXPECT_TRUE(reference > less);
    EXPECT_FALSE(reference > same);
    EXPECT_FALSE(reference > more);
}

/*
 * Test substract method
 */
TEST_F(MoneyTest, Sub)
{
    Money money{50, 40, 20};

    // Simple sub

    money.sub("bronze", 10);
    EXPECT_EQ(money.value("bronze"), 40);
    EXPECT_EQ(money.value("argent"), 40);
    EXPECT_EQ(money.value("or"), 20);

    money.sub("argent", 5);
    EXPECT_EQ(money.value("bronze"), 40);
    EXPECT_EQ(money.value("argent"), 35);
    EXPECT_EQ(money.value("or"), 20);

    money.sub("or", 15);
    EXPECT_EQ(money.value("bronze"), 40);
    EXPECT_EQ(money.value("argent"), 35);
    EXPECT_EQ(money.value("or"), 5);


    // With Carry

    money.sub("bronze", 60);
    EXPECT_EQ(money.value("bronze"), 80);
    EXPECT_EQ(money.value("argent"), 34);
    EXPECT_EQ(money.value("or"), 5);

    money.sub("argent", 39);
    EXPECT_EQ(money.value("bronze"), 80);
    EXPECT_EQ(money.value("argent"), 495);
    EXPECT_EQ(money.value("or"), 4);


    // With huge numbers

    money.sub("bronze", 6005);
    EXPECT_EQ(money.value("bronze"), 75);
    EXPECT_EQ(money.value("argent"), 435);
    EXPECT_EQ(money.value("or"), 4);

    money.sub("bronze", 100005);
    EXPECT_EQ(money.value("bronze"), 70);
    EXPECT_EQ(money.value("argent"), 435);
    EXPECT_EQ(money.value("or"), 2);

    // Not allowed
    money.sub("argent", 1500);
    EXPECT_EQ(money.value("bronze"), 70);
    EXPECT_EQ(money.value("argent"), 435);
    EXPECT_EQ(money.value("or"), 2);

    money.sub("or", 3);
    EXPECT_EQ(money.value("bronze"), 70);
    EXPECT_EQ(money.value("argent"), 435);
    EXPECT_EQ(money.value("or"), 2);

}

/*
 * Test add operators : +, ++, +=
 */
TEST_F(MoneyTest, ArithmeticAddOperators)
{
    Money easy{10, 20, 30};
    Money critical{99, 20, 30};

    // First, operator=
    Money copy = easy;
    EXPECT_EQ(copy, easy);

    Money sum = easy + 30;
    Money expected{40, 20, 30};
    EXPECT_EQ(sum, expected);

    sum = critical + 10;
    expected = Money{9, 21, 30};
    EXPECT_EQ(sum, expected);

    sum = critical + easy;
    expected = Money{9, 41, 60};
    EXPECT_EQ(sum, expected);


    easy += 40;
    expected = Money{50, 20, 30};
    EXPECT_EQ(easy, expected);

    critical += 30;
    expected = Money{29, 21, 30};
    EXPECT_EQ(critical, expected);

    critical += easy;
    expected = Money{79, 41, 60};
    EXPECT_EQ(critical, expected);

    critical = Money{99, 20, 30};

    easy = critical++;
    expected = Money{0, 21, 30};
    EXPECT_EQ(critical, expected);
    EXPECT_EQ(easy, critical);
}

/*
 * Test substract operators : -, --, -=
 */
TEST_F(MoneyTest, ArithmeticSubOperators)
{
    // ----- SUBSTRACT -------

    Money easy{90,50,30};
    Money critical{10,11,40};

    Money sub = easy - 10;
    Money expected{80,50,30};
    EXPECT_EQ(sub, expected);

    sub = critical - 20;
    expected = Money{90, 10, 40};
    EXPECT_EQ(sub, expected);

    sub = easy - Money{10, 11, 20};
    expected = Money{80,39,10};
    EXPECT_EQ(sub, expected);

    easy -= 40;
    expected = Money{50, 50, 30};
    EXPECT_EQ(easy, expected);

    critical -= 30;
    expected = Money{80, 10, 40};
    EXPECT_EQ(critical, expected);

    critical -= easy;
    expected = Money{30, 460, 9};
    EXPECT_EQ(critical, expected);

    critical = Money{0, 20, 30};

    easy = critical--;
    expected = Money{99, 19, 30};
    EXPECT_EQ(critical, expected);
    EXPECT_EQ(easy, critical);

}

/*
 * Test the good printing of the stream operator
 */
TEST_F(MoneyTest, StreamOperator)
{
    Money money{10, 40, 2};
    std::stringstream ss;
    ss << "10 bronze, 40 argent, 2 or" << std::endl;
    ::testing::internal::CaptureStdout();
    std::cout << money << std::endl;
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, ss.str());
}

/**
 * @brief Set up the Money test
 *
 * Except for the "NotInitialized" test
 */
void MoneyTest::SetUp()
{
    Money::m_initialized = false;
    Money::m_moneyNames.clear();

    const ::testing::TestInfo* const test_info = ::testing::UnitTest::GetInstance()->current_test_info();
    auto testName = std::string(test_info->name());

    if (testName != "NotInitialized")
    {
        Money::initialize("bronze",
                          std::pair<std::string, unsigned int>("argent", 100),
                          std::pair<std::string, unsigned int>("or", 50000));
    }
}

/**
 * @brief Reset the statics values to their original value
 */
void MoneyTest::TearDown()
{
    Money::m_initialized = false;
    Money::m_moneyNames.clear();
}

}


