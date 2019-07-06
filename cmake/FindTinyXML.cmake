if (NOT TARGET TinyXML_project)

INCLUDE(ExternalProject)

ExternalProject_Add(
	TinyXML_project

	SOURCE_DIR "${RPG_THIRDPARTY_DIR}/tinyxml"
	PREFIX "${CMAKE_BINARY_DIR}/TinyXML"
	INSTALL_COMMAND ""
	)

endif()
ExternalProject_Get_Property(TinyXML_project source_dir binary_dir)
if (NOT TARGET TinyXML)
	add_library(TinyXML SHARED IMPORTED GLOBAL)
	add_dependencies(TinyXML TinyXML_project)

	set_target_properties(TinyXML PROPERTIES
		IMPORTED_LOCATION "${binary_dir}/libtinyxml2.so"
		INTERFACE_INCLUDE_DIRECTORIES "${source_dir}"
	)
endif()
