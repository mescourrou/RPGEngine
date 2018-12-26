#pragma once

// Project
#include "general_config.hpp"
#include <BaseObject.hpp>

#ifdef RPG_BUILD_TEST
#include <gtest/gtest.h>
#endif

namespace engineGUI
{

#ifdef RPG_BUILD_TEST
class EngineGUITest;
#endif

class EngineGUI : public BaseObject
{
#ifdef RPG_BUILD_TEST
	friend class engineGUI::EngineGUITest;
#endif
public:
	EngineGUI();
	~EngineGUI() override = default;
	
	std::string className() const noexcept override { return "EngineGUI"; }
};

} // namespace engineGUI

