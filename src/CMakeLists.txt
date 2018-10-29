list(APPEND MODULES Config)
list(APPEND MODULES BaseObject)
list(APPEND MODULES Object)

foreach(module ${MODULES})
	add_subdirectory(${module})
	message("Adding " ${module} " to the configuration")
endforeach()


add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} ${MODULES})
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_BINARY_DIR}/${RPG_CONFIG_FILE_DIR}")

