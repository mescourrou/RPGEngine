find_library(glog REQUIRED)

add_library(Tools
        Tools.cpp Tools.hpp
        BaseException.hpp
		Logger.cpp Logger.hpp
		Stringlist.cpp Stringlist.hpp
		StateMachine.cpp StateMachine.hpp
        )
target_link_libraries(Tools glog BaseObject)
target_include_directories(Tools PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(Tools PUBLIC "${CMAKE_BINARY_DIR}/${RPG_CONFIG_FILE_DIR}")

if (RPG_BUILD_TEST)
	find_package(gtest REQUIRED)
	target_link_libraries(Tools gtest)
	add_subdirectory(unit_tests)
endif()

if (RPG_BUILD_GUI)
	add_subdirectory(GUI)
endif()
