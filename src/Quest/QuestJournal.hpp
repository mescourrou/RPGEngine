#pragma once

// Project
#include "general_config.hpp"
#include <BaseObject.hpp>
#include <BaseException.hpp>

#ifdef RPG_BUILD_TEST
#include <gtest/gtest.h>
#endif

namespace quest
{

#ifdef RPG_BUILD_TEST
class QuestJournalTest;
#endif

CREATE_EXCEPTION_CLASS(QuestJournal)

class QuestJournal : public BaseObject
{
    DECLARE_BASEOBJECT(QuestJournal)
#ifdef RPG_BUILD_TEST
	friend class quest::QuestJournalTest;
#endif
public:
	QuestJournal();
    ~QuestJournal() override = default;
};

} // namespace quest

