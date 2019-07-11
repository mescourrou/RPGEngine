find_package(glog REQUIRED)
find_package(gflags REQUIRED)

add_library(Game
    GameLauncher.cpp GameLauncher.hpp
    GameLoader.cpp GameLoader.hpp
    Game.cpp Game.hpp)
target_link_libraries(Game BaseObject
    Config
    Character
    Tools
    Database
    Events
    glog
    gflags
    ${FILESYSTEM_LIB})
target_include_directories(Game PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(Game PUBLIC "${CMAKE_BINARY_DIR}/${RPG_CONFIG_FILE_DIR}")

if (RPG_BUILD_GUI)
    add_subdirectory(GUI)
    target_link_libraries(Game GameGUI)
endif()

if (RPG_BUILD_TEST)
    find_package(gtest REQUIRED)
    target_link_libraries(Game gtest)
    add_subdirectory(unit_tests)
endif()

