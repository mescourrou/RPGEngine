find_package(SFML REQUIRED)
find_package(TinyXML REQUIRED)

add_library(MapGUI
	MapGUI.cpp MapGUI.hpp)
target_link_libraries(MapGUI Map ${SFML_TARGETS} TinyXML)
target_include_directories(MapGUI PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(MapGUI PUBLIC "${CMAKE_BINARY_DIR}/${RPG_CONFIG_FILE_DIR}")
