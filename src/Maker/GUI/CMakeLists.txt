add_library(MakerGUI
	MakerGUI.cpp MakerGUI.hpp)
target_link_libraries(MakerGUI Maker)
target_include_directories(MakerGUI PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(MakerGUI PUBLIC "${CMAKE_BINARY_DIR}/${RPG_CONFIG_FILE_DIR}")
