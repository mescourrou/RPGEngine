find_package(SFML REQUIRED)
find_package(ImGUI-SFML REQUIRED)

add_library(ToolsGUI
	FileBrowser.cpp FileBrowser.hpp
	Popups.cpp Popups.hpp
	Window.cpp Window.hpp
	WindowsManager.cpp WindowsManager.hpp)
target_link_libraries(ToolsGUI
	${SFML_TARGETS} ImGui-SFML
	Tools)


target_include_directories(ToolsGUI PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(ToolsGUI PUBLIC "${CMAKE_BINARY_DIR}/${RPG_CONFIG_FILE_DIR}")
