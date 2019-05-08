add_library(Quest Quest.cpp Quest.hpp)
target_link_libraries(Quest BaseObject)
target_include_directories(Quest PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(Quest PUBLIC "${CMAKE_BINARY_DIR}/${RPG_CONFIG_FILE_DIR}")

if (RPG_BUILD_TEST)
	find_package(gtest REQUIRED)
	target_link_libraries(Quest gtest)
	add_subdirectory(unit_tests)
endif()
	
