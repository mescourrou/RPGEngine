find_package(glog REQUIRED)

add_library(BaseObject BaseObject.cpp BaseObject.hpp)
target_link_libraries(BaseObject glog
    Config)

target_include_directories(BaseObject PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(BaseObject PUBLIC "${CMAKE_BINARY_DIR}/${RPG_CONFIG_FILE_DIR}")

if (RPG_BUILD_GUI)
    add_subdirectory(GUI)
endif()

if (RPG_BUILD_TEST)
    find_package(gtest REQUIRED)
    target_link_libraries(BaseObject gtest)
    add_subdirectory(unit_tests)
endif()

