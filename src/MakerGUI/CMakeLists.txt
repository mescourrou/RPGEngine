add_library(MakerGUI MakerGUI.cpp MakerGUI.hpp)
target_link_libraries(MakerGUI BaseObject)
target_include_directories(MakerGUI PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(MakerGUI PUBLIC "${CMAKE_BINARY_DIR}/${RPG_CONFIG_FILE_DIR}")

if (RPG_BUILD_TEST)
	find_package(gtest REQUIRED)
	target_link_libraries(MakerGUI gtest)
	add_subdirectory(unit_tests)
endif()
	
