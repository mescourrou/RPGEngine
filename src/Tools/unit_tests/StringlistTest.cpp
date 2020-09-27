#include "StringlistTest.hpp"

TEST_F(StringlistTest, copy)
{
    std::vector<std::string> list;
    list.push_back("str1");
    list.push_back("str2");
    list.push_back("str3");
    stringlist slst(list);

    stringlist slst2(slst);
    std::vector<std::string> list2 = slst2.toVectorString();
    ASSERT_EQ(list2.size(), list.size());
    for (unsigned int i = 0; i < list.size(); i++)
    {
        EXPECT_EQ(list.at(i), list2.at(i));
    }
}
