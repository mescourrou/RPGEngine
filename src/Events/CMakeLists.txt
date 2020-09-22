find_package(glog REQUIRED)

add_library(Events
    Event.cpp Event.hpp
    EventLoop.cpp EventLoop.hpp
    WorkerThread.cpp WorkerThread.hpp
	Work.cpp Work.hpp
	ActionHandler.cpp ActionHandler.hpp
	KeyBinding.cpp KeyBinding.hpp)
target_link_libraries(Events BaseObject glog InstrumentationTools)
target_include_directories(Events PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(Events PUBLIC "${CMAKE_BINARY_DIR}/${RPG_CONFIG_FILE_DIR}")

if (RPG_BUILD_GUI)
	find_package(SFML REQUIRED)
	target_link_libraries(Events ${SFML_TARGETS})
endif()

if (RPG_BUILD_TEST)
	find_package(gtest REQUIRED)
	target_link_libraries(Events gtest)
	add_subdirectory(unit_tests)
endif()
	
