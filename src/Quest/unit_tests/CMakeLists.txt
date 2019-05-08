find_package(gtest REQUIRED)

add_executable(Quest_test
	QuestTest.cpp QuestTest.hpp
	QuestJournalTest.cpp QuestJournalTest.hpp
	DialogueTest.cpp DialogueTest.hpp)
target_link_libraries(Quest_test gtest Quest)

if(EXISTS /data AND IS_DIRECTORY /data)
add_custom_command(TARGET Quest_test POST_BUILD
  COMMAND "${CMAKE_COMMAND}" -E copy_directory
	 "${CMAKE_CURRENT_SOURCE_DIR}/data"
	 "${CMAKE_CURRENT_BINARY_DIR}/data"
  COMMENT "Copying test data output directory")
endif()


