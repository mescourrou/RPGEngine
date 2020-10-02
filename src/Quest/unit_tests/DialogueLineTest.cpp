#include "DialogueLineTest.hpp"
namespace quest
{

class TestAction : public DialogueAction
{
  public:
    TestAction() = default;
    ~TestAction() override = default;
    void operator()() override
    {
        done = true;
    }
    bool done = false;
};

TEST_F(DialogueLineTest, ChoiceSelection)
{
    DialogueLine line1("Hello World");
    DialogueLine line2("Do you wanna a quest ?");
    DialogueLine line3("OK, I won't bother you");

    TestAction action;
    line1.addChoice("Greetings", std::make_shared<DialogueLine>(line2));
    line1.addChoice("Ciao", std::make_shared<DialogueLine>(line3),
                    std::make_shared<TestAction>(action));

    // Select "Ciao"
    std::weak_ptr<const DialogueLine> selectedLine = line1.selectChoice(1);
    EXPECT_EQ(line3.line(), selectedLine.lock()->line());
    EXPECT_TRUE(action.done);
}

TEST_F(DialogueLineTest, ChoiceSelectionWhenOnlyOne)
{
    DialogueLine line1("Hello World");
    DialogueLine line2("Do you wanna a quest ?");

    TestAction action;
    line1.addChoice("", std::make_shared<DialogueLine>(line2),
                    std::make_shared<TestAction>(action));

    EXPECT_EQ(line1.choices().size(), 1);
    EXPECT_TRUE(line1.choices().at(0).empty());

    std::weak_ptr<const DialogueLine> selectedLine = line1.selectChoice(0);
    EXPECT_EQ(line2.line(), selectedLine.lock()->line());
    EXPECT_TRUE(action.done);
}

}
