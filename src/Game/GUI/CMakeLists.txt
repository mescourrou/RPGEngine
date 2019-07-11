add_library(GameGUI
    GameGUI.cpp GameGUI.hpp)
target_link_libraries(GameGUI Game
    MapGUI CharacterGUI)
target_include_directories(GameGUI PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(GameGUI PUBLIC "${CMAKE_BINARY_DIR}/${RPG_CONFIG_FILE_DIR}")
