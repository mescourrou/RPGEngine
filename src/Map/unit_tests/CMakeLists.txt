find_package(gtest REQUIRED)
include(GoogleTest)

add_executable(Map_test
    MapTest.cpp MapTest.hpp
    PositionTest.cpp PositionTest.hpp
    VectorTest.cpp VectorTest.hpp
    AreaTest.cpp AreaTest.hpp)
target_link_libraries(Map_test gtest gmock Map Character
    ${FILESYSTEM_LIB})
gtest_discover_tests(Map_test)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/data" AND IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/data")
add_custom_command(TARGET Map_test POST_BUILD
  COMMAND "${CMAKE_COMMAND}" -E copy_directory
     "${CMAKE_CURRENT_SOURCE_DIR}/data"
     "${CMAKE_CURRENT_BINARY_DIR}/data"
  COMMENT "Copying test data output directory")
endif()

add_custom_command(TARGET Map_test
    POST_BUILD
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/..
    COMMAND lcov -c -i -d . -o Coverage.info
    COMMAND genhtml Coverage.info --output-directory ${CMAKE_CURRENT_SOURCE_DIR}/../Coverage
)
