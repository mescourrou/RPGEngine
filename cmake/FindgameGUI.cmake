if (NOT TARGET gameGUI_project)

INCLUDE(ExternalProject)

ExternalProject_Add(
	gameGUI_project

	DOWNLOAD_COMMAND ""
	SOURCE_DIR "${RPG_THIRDPARTY_DIR}/gameGUI"
	PREFIX "${CMAKE_BINARY_DIR}/gameGUI"
	INSTALL_COMMAND ""
	)

endif()
ExternalProject_Get_Property(gameGUI_project source_dir binary_dir)

SET(gameGUILibs)

if (NOT TARGET gameGUI_core)
add_library(gameGUI_core SHARED IMPORTED GLOBAL)
add_dependencies(gameGUI_core gameGUI_project)

set_target_properties(gameGUI_core PROPERTIES
	"IMPORTED_LOCATION" "${binary_dir}/bin/core/libcore.so"
	INTERFACE_INCLUDE_DIRECTORIES "${source_dir}/src"
)
endif()
LIST(APPEND gameGUILibs gameGUI_core)

if (NOT TARGET gameGUI_widgets)
add_library(gameGUI_widgets SHARED IMPORTED GLOBAL)
add_dependencies(gameGUI_widgets gameGUI_project)

set_target_properties(gameGUI_widgets PROPERTIES
	"IMPORTED_LOCATION" "${binary_dir}/bin/widgets/libwidgets.so"
	INTERFACE_INCLUDE_DIRECTORIES "${source_dir}/src"
)
endif()
LIST(APPEND gameGUILibs gameGUI_widgets)
