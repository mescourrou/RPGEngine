find_package(gtest REQUIRED)
include(GoogleTest)

add_executable(Quest_test
    QuestTest.cpp QuestTest.hpp
    QuestJournalTest.cpp QuestJournalTest.hpp
    DialogueTest.cpp DialogueTest.hpp)
target_link_libraries(Quest_test gtest Quest)
gtest_discover_tests(Quest_test)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/data" AND IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/data")
add_custom_command(TARGET Quest_test POST_BUILD
  COMMAND "${CMAKE_COMMAND}" -E copy_directory
     "${CMAKE_CURRENT_SOURCE_DIR}/data"
     "${CMAKE_CURRENT_BINARY_DIR}/data"
  COMMENT "Copying test data output directory")
endif()

add_custom_command(TARGET Quest_test
    POST_BUILD
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/..
    COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/..
    COMMAND find . -name "*.gcda" -or -name "*.gcno" -exec gcov {}  \\\;
#    COMMAND mv *.gcov ${CMAKE_CURRENT_SOURCE_DIR}/../gcov
)
