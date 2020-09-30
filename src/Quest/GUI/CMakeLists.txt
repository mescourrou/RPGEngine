find_package(SFML REQUIRED)
find_package(ImGUI-SFML REQUIRED)

add_library(QuestGUI
    DialogueGUI.cpp DialogueGUI.hpp)
target_link_libraries(QuestGUI Quest ${SFML_TARGETS} ${FILESYSTEM_LIB}
    BaseGUIObject ImGui-SFML
    ToolsGUI)

target_include_directories(QuestGUI PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(QuestGUI PUBLIC "${CMAKE_BINARY_DIR}/${RPG_CONFIG_FILE_DIR}")
