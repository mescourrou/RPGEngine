find_package(gtest REQUIRED)

add_executable(Object_test
	ObjectTest.cpp ObjectTest.hpp
	MoneyTest.cpp MoneyTest.hpp
	InventoryTest.cpp InventoryTest.hpp
	)

target_link_libraries(Object_test gtest Object)

add_custom_command(TARGET Object_test POST_BUILD
  COMMAND "${CMAKE_COMMAND}" -E copy_directory
	 "${CMAKE_CURRENT_SOURCE_DIR}/data"
	 "${CMAKE_CURRENT_BINARY_DIR}/data"
  COMMENT "Copying test data output directory")


