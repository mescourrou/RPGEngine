find_library(glog REQUIRED)
add_library(Object
	Object.cpp Object.hpp
	Money.cpp Money.hpp
	Inventory.cpp Inventory.hpp
	)


target_link_libraries(Object
	BaseObject
	Database
	Tools
	glog)
target_include_directories(Object PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(Object PUBLIC "${CMAKE_BINARY_DIR}/${RPG_CONFIG_FILE_DIR}")

if (RPG_BUILD_GUI)
	add_subdirectory(GUI)
endif()

if (RPG_BUILD_TEST)
	find_package(gtest REQUIRED)
	target_link_libraries(Object gtest)
	add_subdirectory(unit_tests)
endif()

