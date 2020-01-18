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
class QuestTest;
#endif

CREATE_EXCEPTION_CLASS(Quest)

class Quest : public BaseObject
{
    DECLARE_BASEOBJECT(Quest)
#ifdef RPG_BUILD_TEST
    friend class quest::QuestTest;
#endif
  public:
    Quest();
    ~Quest() override = default;
};

} // namespace quest

