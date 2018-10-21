list(APPEND MODULES Config)

foreach(module ${MODULES})
	add_subdirectory(${module})
	message("Adding " ${module} " to the configuration")
endforeach()

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} ${MODULES})
target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/${RPG_CONFIG_FILE_DIR}>)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
