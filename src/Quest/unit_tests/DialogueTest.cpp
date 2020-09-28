#include "DialogueTest.hpp"

#include <Dialogue.hpp>
#include <DialogueLine.hpp>
#include <general_config.hpp>
#include <Database.hpp>
#ifdef BUILD_USE_FILESYSTEM
#include <filesystem>
#endif

namespace quest
{

#ifdef BUILD_USE_FILESYSTEM
TEST_F(DialogueTest, VerifyDatabaseModel)
{
    EXPECT_TRUE(Dialogue::verifyDatabaseModel(database));
}

TEST_F(DialogueTest, LoadFromDatabase)
{
    Dialogue d;
    d.loadFromDatabase(1, database);

    ASSERT_TRUE(d.getFirstLine());
    const DialogueLine* currentLine = d.getFirstLine();
    EXPECT_EQ(currentLine->getLine(), "Hello young knight !");

    EXPECT_EQ(currentLine->getChoices().size(), 1);
    EXPECT_TRUE(currentLine->getChoices().at(0).empty());

    ASSERT_TRUE(currentLine = currentLine->selectChoice(0));
    EXPECT_EQ(currentLine->getLine(), "Are you interested in a quest ?");

    EXPECT_EQ(currentLine->getChoices().size(), 2);
    EXPECT_EQ(currentLine->getChoices().at(0), "Yes, I would !");
    EXPECT_EQ(currentLine->getChoices().at(1), "Go away !");

    const DialogueLine* selectLine0 = currentLine->selectChoice(0);
    ASSERT_TRUE(selectLine0);
    EXPECT_EQ(selectLine0->getLine(), "Here we go.");

    const DialogueLine* selectLine1 = currentLine->selectChoice(1);
    ASSERT_TRUE(selectLine0);
    EXPECT_EQ(selectLine1->getLine(), "Alright, have a good day !");

}

TEST_F(DialogueTest, LoadFromDatabaseMultiple)
{
    std::vector<Dialogue> list = Dialogue::loadFromDatabase("John", database);
    EXPECT_EQ(list.size(), 2);
    ASSERT_TRUE(list.at(0).getFirstLine());
    EXPECT_EQ(list.at(0).getFirstLine()->getLine(), "Hello young knight !");

    EXPECT_TRUE(list.at(1).getFirstLine());
    EXPECT_EQ(list.at(1).getFirstLine()->getLine(), "Hi !");
}

/**
 * @brief Prepare the database usage
 */
void DialogueTest::SetUp()
{
    std::filesystem::path usedFile = "data/dialogue_data.db";
    std::filesystem::path modelFile = "data/dialogue_data.sqlite";
    std::filesystem::copy(modelFile, usedFile,
                          std::filesystem::copy_options::overwrite_existing);

    database = std::shared_ptr<databaseTools::Database>(new databaseTools::Database(
                   usedFile));
}

#endif
}
