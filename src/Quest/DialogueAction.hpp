#pragma once

// Project
#include "general_config.hpp"
#include <BaseObject.hpp>

#ifdef RPG_BUILD_TEST
#include <gtest/gtest.h>
#endif

namespace quest
{

#ifdef RPG_BUILD_TEST
class DialogueActionTest;
#endif

/**
 * @brief Base action class for the DialogueLine choices.
 */
class DialogueAction : public BaseObject
{
    DECLARE_BASEOBJECT(DialogueAction)
#ifdef RPG_BUILD_TEST
    friend class quest::DialogueActionTest;
#endif
  public:
    DialogueAction() = default;
    virtual ~DialogueAction() override = default;

    virtual void operator()() = 0;
};

} // namespace quest

