cmake_minimum_required(VERSION 3.10)
project(RPG)

option(RPG_BUILD_TEST "Compile Unit Tests" ON)
option(RPG_COMPILE_THIRDPARTIES "Compile third-party libs" ON)

set(RPG_THIRDPARTY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty")

set(RPG_CONFIG_FILE_DIR "config_files" CACHE STRING "Directory containing template for configure_file")

set(RPG_VERBOSE 10 CACHE STRING "Level of verbosity")

set(SOURCES "")

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (RPG_BUILD_TEST)
	enable_testing()
endif()

add_subdirectory(${RPG_CONFIG_FILE_DIR})
add_subdirectory(${SOURCE_DIR} bin)



