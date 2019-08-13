add_library(epstl INTERFACE)
target_include_directories(epstl INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/")

if (RPG_BUILD_TEST)
	find_package(gtest REQUIRED)
	target_link_libraries(epstl INTERFACE gtest)
	add_subdirectory(unit_tests)
endif()
	
