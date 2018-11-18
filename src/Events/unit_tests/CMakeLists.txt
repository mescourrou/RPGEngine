find_package(gtest REQUIRED)

add_executable(Events_test
    EventTest.cpp EventTest.hpp
    EventLoopTest.cpp EventLoopTest.hpp
    WorkerThreadTest.cpp WorkerThreadTest.hpp
    WorkTest.cpp WorkTest.hpp)
target_link_libraries(Events_test gtest Events)

add_custom_command(TARGET Events_test POST_BUILD
  COMMAND "${CMAKE_COMMAND}" -E copy_directory
	 "${CMAKE_CURRENT_SOURCE_DIR}/data"
	 "${CMAKE_CURRENT_BINARY_DIR}/data"
  COMMENT "Copying test data output directory")


