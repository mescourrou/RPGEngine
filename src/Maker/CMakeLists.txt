set(MakerName "${PROJECT_NAME}Maker")

add_executable(${MakerName} main.cpp)
target_link_libraries(${MakerName}
	#Game
	)
target_include_directories(${MakerName} PUBLIC "${CMAKE_BINARY_DIR}/${RPG_CONFIG_FILE_DIR}")
