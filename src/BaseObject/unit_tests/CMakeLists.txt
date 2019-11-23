find_package(gtest REQUIRED)
include(GoogleTest)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(BaseObject_test BaseObjectTest.cpp BaseObjectTest.hpp)
target_link_libraries(BaseObject_test gtest BaseObject)
gtest_discover_tests(BaseObject_test)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/data" AND IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/data")
add_custom_command(TARGET BaseObject_test POST_BUILD
  COMMAND "${CMAKE_COMMAND}" -E copy_directory
     "${CMAKE_CURRENT_SOURCE_DIR}/data"
     "${CMAKE_CURRENT_BINARY_DIR}/data"
  COMMENT "Copying test data output directory")
endif()

set_property(TARGET BaseObject_test PROPERTY CXX_STANDARD 17)
add_custom_command(TARGET BaseObject_test
    POST_BUILD
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/..
    COMMAND lcov -c -i -d . -o Coverage.info
    COMMAND genhtml Coverage.info --output-directory ${CMAKE_CURRENT_SOURCE_DIR}/../Coverage
)
