find_package(SFML REQUIRED)
find_package(ImGUI-SFML REQUIRED)

add_library(MakerGUI
	MakerGUI.cpp MakerGUI.hpp)
target_link_libraries(MakerGUI Maker ToolsGUI
	${SFML_TARGETS} ImGui-SFML)


target_include_directories(MakerGUI PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(MakerGUI PUBLIC "${CMAKE_BINARY_DIR}/${RPG_CONFIG_FILE_DIR}")
