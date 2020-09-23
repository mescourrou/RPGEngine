find_package(glog REQUIRED)
find_package(json REQUIRED)

add_library(Map
    Map.cpp Map.hpp
    Position.cpp Position.hpp
    Vector.cpp Vector.hpp
    Area.cpp Area.hpp)
target_link_libraries(Map
    BaseObject
    DatabaseTools
    glog
    json
    Config)
target_include_directories(Map PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(Map PUBLIC "${CMAKE_BINARY_DIR}/${RPG_CONFIG_FILE_DIR}")

if (RPG_BUILD_GUI)
    add_subdirectory(GUI)
endif()

if (RPG_BUILD_TEST)
    find_package(gtest REQUIRED)
    target_link_libraries(Map gtest)
    add_subdirectory(unit_tests)
endif()

