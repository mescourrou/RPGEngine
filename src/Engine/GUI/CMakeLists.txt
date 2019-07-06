add_library(EngineGUI
	EngineGUI.cpp EngineGUI.hpp)
target_link_libraries(EngineGUI Engine
	Tools
	MapGUI)
target_include_directories(EngineGUI PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(EngineGUI PUBLIC "${CMAKE_BINARY_DIR}/${RPG_CONFIG_FILE_DIR}")
