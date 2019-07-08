find_package(SFML REQUIRED)

add_library(CharacterGUI
	CharacterGUI.cpp CharacterGUI.hpp)
target_link_libraries(CharacterGUI Character ${SFML_TARGETS}
    GameGUI)
target_include_directories(CharacterGUI PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(CharacterGUI PUBLIC "${CMAKE_BINARY_DIR}/${RPG_CONFIG_FILE_DIR}")
