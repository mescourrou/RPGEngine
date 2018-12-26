#pragma once

// Project
#include "general_config.hpp"
#include <BaseObject.hpp>

#ifdef RPG_BUILD_TEST
#include <gtest/gtest.h>
#endif

namespace makerGUI
{

#ifdef RPG_BUILD_TEST
class MakerGUITest;
#endif

class MakerGUI : public BaseObject
{
#ifdef RPG_BUILD_TEST
	friend class makerGUI::MakerGUITest;
#endif
public:
	MakerGUI();
	~MakerGUI() override = default;
	
	std::string className() const noexcept override { return "MakerGUI"; }
};

} // namespace makerGUI

