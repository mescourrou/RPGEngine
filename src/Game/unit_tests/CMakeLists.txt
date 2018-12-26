find_package(gtest REQUIRED)

add_executable(Game_test
	GameLauncherTest.cpp GameLauncherTest.hpp
	GameLoaderTest.cpp GameLoaderTest.hpp
	GameTest.cpp GameTest.hpp)
target_link_libraries(Game_test gtest Game)

if(EXISTS /data AND IS_DIRECTORY /data)
add_custom_command(TARGET Game_test POST_BUILD
  COMMAND "${CMAKE_COMMAND}" -E copy_directory
	 "${CMAKE_CURRENT_SOURCE_DIR}/data"
	 "${CMAKE_CURRENT_BINARY_DIR}/data"
  COMMENT "Copying test data output directory")
endif()


