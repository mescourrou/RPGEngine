cmake_minimum_required(VERSION 2.8)

project(RPG)

option(RPG_COMPILE_TEST "Compile Unit Tests" ON)
option(RPG_COMPILE_THIRDPARTIES "Compile third-party libs" ON)

set(RPG_CONFIG_FILE_DIR config_files CACHE STRING "Directory containing template for configure_file")

set(RPG_VERBOSE 10 CACHE STRING "Level of verbosity")

set(SOURCES "")

add_subdirectory(${RPG_CONFIG_FILE_DIR})
add_subdirectory(src bin)

if (RPG_COMPILE_TEST)
	add_subdirectory(tests)

endif()


