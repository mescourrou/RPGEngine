find_package(gtest REQUIRED)

add_executable(Tools_test
	ToolsTest.cpp ToolsTest.hpp)
target_link_libraries(Tools_test gtest Tools)

add_custom_command(TARGET Tools_test POST_BUILD
  COMMAND "${CMAKE_COMMAND}" -E copy_directory
	 "${CMAKE_CURRENT_SOURCE_DIR}/data"
	 "${CMAKE_CURRENT_BINARY_DIR}/data"
  COMMENT "Copying test data output directory")


