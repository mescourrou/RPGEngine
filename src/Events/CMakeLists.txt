add_library(Events
    Event.cpp Event.hpp
    EventLoop.cpp EventLoop.hpp
    WorkerThread.cpp WorkerThread.hpp
    Work.cpp Work.hpp)
target_link_libraries(Events BaseObject)
target_include_directories(Events PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(Events PUBLIC "${CMAKE_BINARY_DIR}/${RPG_CONFIG_FILE_DIR}")

if (RPG_BUILD_TEST)
	find_package(gtest REQUIRED)
	target_link_libraries(Events gtest)
	add_subdirectory(unit_tests)
endif()
	
