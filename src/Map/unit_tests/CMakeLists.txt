find_package(gtest REQUIRED)

add_executable(Map_test
	MapTest.cpp MapTest.hpp
	PositionTest.cpp PositionTest.hpp
	VectorTest.cpp VectorTest.hpp)
target_link_libraries(Map_test gtest Map)

add_custom_command(TARGET Map_test POST_BUILD
  COMMAND "${CMAKE_COMMAND}" -E copy_directory
	 "${CMAKE_CURRENT_SOURCE_DIR}/data"
	 "${CMAKE_CURRENT_BINARY_DIR}/data"
  COMMENT "Copying test data output directory")


