find_package(SFML REQUIRED)
find_package(ImGUI-SFML REQUIRED)

add_library(GameGUI
	GameGUI.cpp GameGUI.hpp
	CharacterWindow.cpp CharacterWindow.hpp
	InventoryWindow.cpp InventoryWindow.hpp)
target_link_libraries(GameGUI Game
	MapGUI CharacterGUI ImGui-SFML
	ToolsGUI)
target_include_directories(GameGUI PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(GameGUI PUBLIC "${CMAKE_BINARY_DIR}/${RPG_CONFIG_FILE_DIR}")
