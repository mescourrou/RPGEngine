set(EngineName "${PROJECT_NAME}Engine")

add_executable(${EngineName} main.cpp)
target_link_libraries(${EngineName}
	Game)
target_include_directories(${EngineName} PUBLIC "${CMAKE_BINARY_DIR}/${RPG_CONFIG_FILE_DIR}")

if (RPG_BUILD_GUI)
	add_subdirectory(GUI)
endif()
