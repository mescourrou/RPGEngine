find_package(LuaBridge REQUIRED)
find_library(lua REQUIRED)


add_library(Scripting
    LuaLoader.cpp LuaLoader.hpp
    LuaManager.cpp LuaManager.hpp)
target_link_libraries(Scripting BaseObject LuaBridge lua)
target_include_directories(Scripting PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(Scripting PUBLIC "${CMAKE_BINARY_DIR}/${RPG_CONFIG_FILE_DIR}")

if (RPG_BUILD_TEST)
    find_package(gtest REQUIRED)
    target_link_libraries(Scripting gtest)
    add_subdirectory(unit_tests)
endif()

