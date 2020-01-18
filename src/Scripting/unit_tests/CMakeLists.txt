find_package(gtest REQUIRED)

add_executable(Scripting_test ScriptingTest.cpp
    LuaLoaderTest.cpp LuaLoaderTest.hpp)
target_link_libraries(Scripting_test gtest Scripting)

if(EXISTS /data AND IS_DIRECTORY /data)
add_custom_command(TARGET Scripting_test POST_BUILD
  COMMAND "${CMAKE_COMMAND}" -E copy_directory
     "${CMAKE_CURRENT_SOURCE_DIR}/data"
     "${CMAKE_CURRENT_BINARY_DIR}/data"
  COMMENT "Copying test data output directory")
endif()


