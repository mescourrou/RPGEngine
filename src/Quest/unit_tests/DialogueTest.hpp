#pragma once

#include <gtest/gtest.h>
#include <Dialogue.hpp>

namespace quest
{

class DialogueTest : public testing::Test
{
  public:
    void SetUp();

  protected:
    std::shared_ptr<databaseTools::Database> database;
};

}

