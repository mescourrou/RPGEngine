find_package(gtest REQUIRED)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(BaseObject_test BaseObjectTest.cpp BaseObjectTest.hpp)
target_link_libraries(BaseObject_test gtest BaseObject)

add_custom_command(TARGET BaseObject_test POST_BUILD
  COMMAND "${CMAKE_COMMAND}" -E copy_directory
	 "${CMAKE_CURRENT_SOURCE_DIR}/data"
	 "${CMAKE_CURRENT_BINARY_DIR}/data"
  COMMENT "Copying test data output directory")

set_property(TARGET BaseObject_test PROPERTY CXX_STANDARD 17)
