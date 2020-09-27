find_package(gtest REQUIRED)
include(GoogleTest)

find_package(json REQUIRED)

add_executable(Tools_test
        ToolsTest.cpp ToolsTest.hpp
        StringlistTest.cpp StringlistTest.hpp)
target_link_libraries(Tools_test gtest json Tools)
gtest_discover_tests(Tools_test)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/data" AND IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/data")
add_custom_command(TARGET Tools_test POST_BUILD
  COMMAND "${CMAKE_COMMAND}" -E copy_directory
     "${CMAKE_CURRENT_SOURCE_DIR}/data"
     "${CMAKE_CURRENT_BINARY_DIR}/data"
  COMMENT "Copying test data output directory")
endif()

add_custom_command(TARGET Tools_test
    POST_BUILD
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/..
    COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/..
    COMMAND find . -name "*.gcda" -or -name "*.gcno" -exec gcov {}  \\\;
#    COMMAND mv *.gcov ${CMAKE_CURRENT_SOURCE_DIR}/../gcov
)
