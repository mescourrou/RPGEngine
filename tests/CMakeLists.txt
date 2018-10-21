
list(APPEND TEST_SOURCES main.cpp)
add_executable(${PROJECT_NAME}_tests ${TEST_SOURCES})
target_link_libraries(${PROJECT_NAME}_tests Config)


enable_testing()
add_test(Test ${PROJECT_NAME}_tests)
