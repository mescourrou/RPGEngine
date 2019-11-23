find_package(gtest REQUIRED)
include(GoogleTest)

add_executable(Character_test
    CharacterTest.cpp CharacterTest.hpp
    VendorTest.cpp VendorTest.hpp)
target_link_libraries(Character_test gtest gmock Character ${FILESYSTEM_LIB})
gtest_discover_tests(Character_test)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/data" AND IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/data")
add_custom_command(TARGET Character_test POST_BUILD
  COMMAND "${CMAKE_COMMAND}" -E copy_directory
     "${CMAKE_CURRENT_SOURCE_DIR}/data"
     "${CMAKE_CURRENT_BINARY_DIR}/data"
  COMMENT "Copying test data output directory")
endif()

add_custom_command(TARGET Character_test
    POST_BUILD
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/..
    COMMAND find . -name "*.gcda" -exec gcov {}  \\\;
    COMMAND mkdir -p ${CMAKE_CURRENT_SOURCE_DIR}/../gcov
    COMMAND mv *.gcov ${CMAKE_CURRENT_SOURCE_DIR}/../gcov
)
