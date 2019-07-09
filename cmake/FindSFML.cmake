if (NOT TARGET sfml_project)

INCLUDE(ExternalProject)

ExternalProject_Add(
	sfml_project

	SOURCE_DIR "${RPG_THIRDPARTY_DIR}/sfml"
	PREFIX "${CMAKE_BINARY_DIR}/SFML"
	INSTALL_COMMAND ""
	)

endif()
ExternalProject_Get_Property(sfml_project source_dir binary_dir)

set(SFML_TARGETS)

if (NOT TARGET sfml_audio)
	add_library(sfml_audio SHARED IMPORTED GLOBAL)
	add_dependencies(sfml_audio sfml_project)

	set_target_properties(sfml_audio PROPERTIES
		IMPORTED_LOCATION "${binary_dir}/lib/libsfml-audio.so"
		INTERFACE_INCLUDE_DIRECTORIES "${source_dir}/include"
    )
endif()
list(APPEND SFML_TARGETS sfml_audio)

if (NOT TARGET sfml_graphics)
	add_library(sfml_graphics SHARED IMPORTED GLOBAL)
	add_dependencies(sfml_graphics sfml_project)

	set_target_properties(sfml_graphics PROPERTIES
		IMPORTED_LOCATION "${binary_dir}/lib/libsfml-graphics.so"
		INTERFACE_INCLUDE_DIRECTORIES "${source_dir}/include"
    )
endif()
list(APPEND SFML_TARGETS sfml_graphics)

if (NOT TARGET sfml_network)
	add_library(sfml_network SHARED IMPORTED GLOBAL)
	add_dependencies(sfml_network sfml_project)

	set_target_properties(sfml_network PROPERTIES
		IMPORTED_LOCATION "${binary_dir}/lib/libsfml-network.so"
		INTERFACE_INCLUDE_DIRECTORIES "${source_dir}/include"
    )
endif()
list(APPEND SFML_TARGETS sfml_graphics)

if (NOT TARGET sfml_system)
	add_library(sfml_system SHARED IMPORTED GLOBAL)
	add_dependencies(sfml_system sfml_project)

	set_target_properties(sfml_system PROPERTIES
		IMPORTED_LOCATION "${binary_dir}/lib/libsfml-system.so"
		INTERFACE_INCLUDE_DIRECTORIES "${source_dir}/include"
    )
endif()
list(APPEND SFML_TARGETS sfml_system)

if (NOT TARGET sfml_window)
	add_library(sfml_window SHARED IMPORTED GLOBAL)
	add_dependencies(sfml_window sfml_project)

	set_target_properties(sfml_window PROPERTIES
		IMPORTED_LOCATION "${binary_dir}/lib/libsfml-window.so"
		INTERFACE_INCLUDE_DIRECTORIES "${source_dir}/include"
    )
endif()
list(APPEND SFML_TARGETS sfml_window)
