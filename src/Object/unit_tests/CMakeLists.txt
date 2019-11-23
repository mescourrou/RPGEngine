find_package(gtest REQUIRED)
include(GoogleTest)

add_executable(Object_test
    ObjectTest.cpp ObjectTest.hpp
    MoneyTest.cpp MoneyTest.hpp
    InventoryTest.cpp InventoryTest.hpp
    )

target_link_libraries(Object_test gtest Object Character
    ${FILESYSTEM_LIB})
gtest_discover_tests(Object_test)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/data" AND IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/data")
add_custom_command(TARGET Object_test POST_BUILD
  COMMAND "${CMAKE_COMMAND}" -E copy_directory
     "${CMAKE_CURRENT_SOURCE_DIR}/data"
     "${CMAKE_CURRENT_BINARY_DIR}/data"
  COMMENT "Copying test data output directory")
endif()

add_custom_command(TARGET Object_test
    POST_BUILD
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/..
    COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/..
    COMMAND find . -name "*.gcda" -or -name "*.gcno" -exec gcov {}  \\\;
#    COMMAND mv *.gcov ${CMAKE_CURRENT_SOURCE_DIR}/../gcov
)
