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

    ASSERT_TRUE(d.firstLine());
    const DialogueLine* currentLine = d.firstLine();
    EXPECT_EQ(currentLine->line(), "Hello young knight !");

    EXPECT_EQ(currentLine->choices().size(), 1);
    EXPECT_TRUE(currentLine->choices().at(0).empty());

    ASSERT_TRUE(currentLine = currentLine->selectChoice(0));
    EXPECT_EQ(currentLine->line(), "Are you interested in a quest ?");

    EXPECT_EQ(currentLine->choices().size(), 2);
    EXPECT_EQ(currentLine->choices().at(0), "Yes, I would !");
    EXPECT_EQ(currentLine->choices().at(1), "Go away !");

    const DialogueLine* selectLine0 = currentLine->selectChoice(0);
    ASSERT_TRUE(selectLine0);
    EXPECT_EQ(selectLine0->line(), "Here we go.");

    const DialogueLine* selectLine1 = currentLine->selectChoice(1);
    ASSERT_TRUE(selectLine0);
    EXPECT_EQ(selectLine1->line(), "Alright, have a good day !");

}

TEST_F(DialogueTest, LoadFromDatabaseMultiple)
{
    std::vector<Dialogue> list = Dialogue::loadFromDatabase("John", database);
    EXPECT_EQ(list.size(), 2);
    ASSERT_TRUE(list.at(0).firstLine());
    EXPECT_EQ(list.at(0).firstLine()->line(), "Hello young knight !");

    ASSERT_TRUE(list.at(1).firstLine());
    EXPECT_EQ(list.at(1).firstLine()->line(), "Hi !");
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

    database = std::make_shared<databaseTools::Database>(usedFile);
}

#endif
}
