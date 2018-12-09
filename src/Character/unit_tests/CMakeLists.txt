find_package(gtest REQUIRED)

add_executable(Character_test CharacterTest.cpp CharacterTest.hpp)
target_link_libraries(Character_test gtest Character stdc++fs)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/data" AND IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/data")
add_custom_command(TARGET Character_test POST_BUILD
  COMMAND "${CMAKE_COMMAND}" -E copy_directory
	 "${CMAKE_CURRENT_SOURCE_DIR}/data"
	 "${CMAKE_CURRENT_BINARY_DIR}/data"
  COMMENT "Copying test data output directory")
endif()

