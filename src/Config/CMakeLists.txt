find_package(simpleini REQUIRED)
find_package(glog REQUIRED)


add_library(Config
	Config.cpp Config.hpp
	Context.cpp Context.hpp
	VerbosityLevels.hpp)
target_link_libraries(Config simpleini glog
	BaseObject
	Tools
	stdc++fs)
target_include_directories(Config PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(Config PUBLIC "${CMAKE_BINARY_DIR}/${RPG_CONFIG_FILE_DIR}")

if (RPG_BUILD_TEST)
	find_package(gtest REQUIRED)
	target_link_libraries(Config gtest)
	add_subdirectory(unit_tests)
endif()

