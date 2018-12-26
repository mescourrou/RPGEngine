find_package(gtest REQUIRED)

add_executable(MakerGUI_test MakerGUITest.cpp MakerGUITest.hpp)
target_link_libraries(MakerGUI_test gtest MakerGUI)

if(EXISTS /data AND IS_DIRECTORY /data)
add_custom_command(TARGET MakerGUI_test POST_BUILD
  COMMAND "${CMAKE_COMMAND}" -E copy_directory
	 "${CMAKE_CURRENT_SOURCE_DIR}/data"
	 "${CMAKE_CURRENT_BINARY_DIR}/data"
  COMMENT "Copying test data output directory")
endif()


