find_package(glog REQUIRED)
find_package(sqlite REQUIRED)

add_library(Database
	Database.cpp Database.hpp
	Query.cpp Query.hpp)
target_link_libraries(Database glog BaseObject sqlite)
target_include_directories(Database PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(Database PUBLIC "${CMAKE_BINARY_DIR}/${RPG_CONFIG_FILE_DIR}")

if (RPG_BUILD_TEST)
	find_package(gtest REQUIRED)
	target_link_libraries(Database gtest)
	add_subdirectory(unit_tests)
endif()

