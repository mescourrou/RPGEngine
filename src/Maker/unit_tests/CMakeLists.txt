find_package(gtest REQUIRED)
find_package(glog REQUIRED)

add_executable(Maker_test
	MakerTest.cpp MakerTest.hpp
	)

target_link_libraries(Maker_test gtest Maker
	glog
	stdc++fs)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/data" AND IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/data")
add_custom_command(TARGET Maker_test POST_BUILD
  COMMAND "${CMAKE_COMMAND}" -E copy_directory
	 "${CMAKE_CURRENT_SOURCE_DIR}/data"
	 "${CMAKE_CURRENT_BINARY_DIR}/data"
  COMMENT "Copying test data output directory")
endif()

