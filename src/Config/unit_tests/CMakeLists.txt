find_package(gtest REQUIRED)

add_executable(Config_test
    ConfigTest.cpp ConfigTest.hpp
    ContextTest.cpp ContextTest.hpp)
target_link_libraries(Config_test gtest gmock Config)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/data" AND IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/data")
add_custom_command(TARGET Config_test POST_BUILD
  COMMAND "${CMAKE_COMMAND}" -E copy_directory
     "${CMAKE_CURRENT_SOURCE_DIR}/data"
     "${CMAKE_CURRENT_BINARY_DIR}/data"
  COMMENT "Copying test data output directory")
endif()
