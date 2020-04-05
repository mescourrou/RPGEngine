find_library(glog REQUIRED)

add_library(Character
    Character.cpp Character.hpp
    NPC.cpp NPC.hpp
    Vendor.cpp Vendor.hpp)
target_link_libraries(Character
    glog BaseObject
    DatabaseTools
    Object
    Tools
    Events
    Map)
target_include_directories(Character PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(Character PUBLIC "${CMAKE_BINARY_DIR}/${RPG_CONFIG_FILE_DIR}")

if (RPG_BUILD_GUI)
    add_subdirectory(GUI)
endif()

if (RPG_BUILD_TEST)
    find_package(gtest REQUIRED)
    target_link_libraries(Character gtest)
    add_subdirectory(unit_tests)
endif()

