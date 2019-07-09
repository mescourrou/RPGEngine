set(MakerName "${PROJECT_NAME}Maker")

find_package(glog REQUIRED)

add_library(Maker
    Maker.cpp Maker.hpp)
target_link_libraries(Maker
    Database
    Events
    Character Map Game Config Object Tools
    glog
    ${FILESYSTEM_LIB})
target_include_directories(Maker PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(Maker PUBLIC "${CMAKE_BINARY_DIR}/${RPG_CONFIG_FILE_DIR}")

if (RPG_BUILD_GUI)
    add_subdirectory(GUI)
endif()

if (RPG_BUILD_TEST)
    find_package(gtest REQUIRED)
    target_link_libraries(Maker gtest)
    add_subdirectory(unit_tests)
endif()

add_executable(${MakerName} main.cpp)
target_link_libraries(${MakerName}
    Maker
    )
target_include_directories(${MakerName} PUBLIC "${CMAKE_BINARY_DIR}/${RPG_CONFIG_FILE_DIR}")
