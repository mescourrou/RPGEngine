find_package(SFML REQUIRED)
find_package(ImGUI-SFML REQUIRED)
find_package(ImNodes REQUIRED)

add_library(MakerGUI
    MakerGUI.cpp MakerGUI.hpp
    CharacterWindow.cpp CharacterWindow.hpp
    ConsoleWindow.cpp ConsoleWindow.hpp
    MoneyWindow.cpp MoneyWindow.hpp
    MapWindow.cpp MapWindow.hpp
    DialogueWindow.cpp DialogueWindow.hpp
    DialogueLineEditWindow.cpp DialogueLineEditWindow.hpp)

target_link_libraries(MakerGUI Maker ToolsGUI
    ${SFML_TARGETS} ImGui-SFML ImNodes)


target_include_directories(MakerGUI PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(MakerGUI PUBLIC "${CMAKE_BINARY_DIR}/${RPG_CONFIG_FILE_DIR}")
