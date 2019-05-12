set(EngineName "${PROJECT_NAME}Engine")

add_executable(${EngineName} main.cpp)
target_link_libraries(${EngineName}
	Game)
target_include_directories(${EngineName} PUBLIC "${CMAKE_BINARY_DIR}/${RPG_CONFIG_FILE_DIR}")

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/config" AND IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/config")
add_custom_command(TARGET ${EngineName} POST_BUILD
  COMMAND "${CMAKE_COMMAND}" -E copy_directory
	 "${CMAKE_CURRENT_SOURCE_DIR}/config"
	 "${CMAKE_CURRENT_BINARY_DIR}/config"
  COMMENT "Copying config directory")
message("Copying config directory")
endif()

if (RPG_BUILD_GUI)
	add_subdirectory(GUI)
endif()
