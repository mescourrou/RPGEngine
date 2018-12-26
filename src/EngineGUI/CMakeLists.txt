add_library(EngineGUI EngineGUI.cpp EngineGUI.hpp)
target_link_libraries(EngineGUI BaseObject)
target_include_directories(EngineGUI PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(EngineGUI PUBLIC "${CMAKE_BINARY_DIR}/${RPG_CONFIG_FILE_DIR}")

if (RPG_BUILD_TEST)
	find_package(gtest REQUIRED)
	target_link_libraries(EngineGUI gtest)
	add_subdirectory(unit_tests)
endif()
	
