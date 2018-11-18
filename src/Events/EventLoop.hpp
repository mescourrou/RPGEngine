#pragma once

// Project
#include "general_config.hpp"
#include <BaseObject.hpp>

#ifdef RPG_BUILD_TEST
#include <gtest/gtest.h>
#endif

namespace events
{

#ifdef RPG_BUILD_TEST
class EventLoopTest;
#endif

class EventLoop : public BaseObject
{
#ifdef RPG_BUILD_TEST
	friend class events::EventLoopTest;
#endif
public:
	EventLoop();
	~EventLoop() override = default;

	std::string className() const noexcept override { return "EventLoop"; }
};

} // namespace events

