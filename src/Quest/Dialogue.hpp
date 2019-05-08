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
class DialogueTest;
#endif

CREATE_EXCEPTION_CLASS(Dialogue)

class Dialogue : public BaseObject
{
    DECLARE_BASEOBJECT(Dialogue)
#ifdef RPG_BUILD_TEST
	friend class quest::DialogueTest;
#endif
public:
	Dialogue();
    ~Dialogue() override = default;
};

} // namespace quest

