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
    auto line1 = std::make_shared<DialogueLine>("Hello World");
    auto line2 = std::make_shared<DialogueLine>("Do you wanna a quest ?");
    auto line3 = std::make_shared<DialogueLine>("OK, I won't bother you");

    auto action = std::make_shared<TestAction>();
    line1->addChoice("Greetings", line2);
    line1->addChoice("Ciao", line3, action);

    // Select "Ciao"
    std::weak_ptr<const DialogueLine> selectedLine = line1->selectChoice(1);
    EXPECT_EQ(line3->line(), selectedLine.lock()->line());
    EXPECT_TRUE(action->done);
}

TEST_F(DialogueLineTest, ChoiceSelectionWhenOnlyOne)
{
    auto line1 = std::make_shared<DialogueLine>("Hello World");
    auto line2 = std::make_shared<DialogueLine>("Do you wanna a quest ?");

    auto action = std::make_shared<TestAction>();
    line1->addChoice("", line2, action);

    EXPECT_EQ(line1->choices().size(), 1);
    EXPECT_TRUE(line1->choices().at(0).empty());

    std::weak_ptr<const DialogueLine> selectedLine = line1->selectChoice(0);
    EXPECT_EQ(line2->line(), selectedLine.lock()->line());
    EXPECT_TRUE(action->done);
}

}
