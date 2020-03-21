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

    TestAction* action = new TestAction;
    line1.addChoice("Greetings", &line2);
    line1.addChoice("Ciao", &line3, action);

    const DialogueLine* selectedLine = line1.selectChoice(1); // Select "Ciao"
    EXPECT_EQ(&line3, selectedLine);
    EXPECT_TRUE(action->done);
}

}
