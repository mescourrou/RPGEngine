add_library(ObjectGUI
	ObjectGUI.cpp ObjectGUI.hpp)
target_link_libraries(ObjectGUI Object)
target_include_directories(ObjectGUI PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(ObjectGUI PUBLIC "${CMAKE_BINARY_DIR}/${RPG_CONFIG_FILE_DIR}")
