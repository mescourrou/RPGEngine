find_package(gtest REQUIRED)

add_executable(EngineGUI_test EngineGUITest.cpp EngineGUITest.hpp)
target_link_libraries(EngineGUI_test gtest EngineGUI)

if(EXISTS /data AND IS_DIRECTORY /data)
add_custom_command(TARGET EngineGUI_test POST_BUILD
  COMMAND "${CMAKE_COMMAND}" -E copy_directory
	 "${CMAKE_CURRENT_SOURCE_DIR}/data"
	 "${CMAKE_CURRENT_BINARY_DIR}/data"
  COMMENT "Copying test data output directory")
endif()


