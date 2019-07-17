find_package(SFML REQUIRED)

if (NOT TARGET imgui-SFML_project)

INCLUDE(ExternalProject)

ExternalProject_Get_Property(sfml_project source_dir binary_dir)

set(SFML_BUILD ${binary_dir})
message("SFML build dir : ${SFML_BUILD}")
if (NOT EXISTS ${SFML_BUILD})
	file(MAKE_DIRECTORY ${SFML_BUILD})
endif()

ExternalProject_Add(
	imgui-SFML_project
    DOWNLOAD_COMMAND ""
    SOURCE_DIR "${RPG_THIRDPARTY_DIR}/imgui-SFML"
    PREFIX "${CMAKE_BINARY_DIR}/imgui-SFML"
	CMAKE_ARGS -DIMGUI_DIR=${RPG_THIRDPARTY_DIR}/imgui -DSFML_DIR=${SFML_BUILD}
    INSTALL_COMMAND ""
	)
ExternalProject_Add_Step(imgui-SFML_project sfmlBuild
	DEPENDERS mkdir)
ExternalProject_Add_StepDependencies(imgui-SFML_project download sfml_project ${SFML_TARGETS})


endif()

ExternalProject_Get_Property(imgui-SFML_project source_dir binary_dir)

if (NOT TARGET ImGui-SFML)
	find_library(OpenGL REQUIRED)
	add_library(ImGui-SFML STATIC IMPORTED GLOBAL)
	add_dependencies(ImGui-SFML imgui-SFML_project)

	set_target_properties(ImGui-SFML PROPERTIES
	"IMPORTED_LOCATION" "${binary_dir}/libImGui-SFML.a"
	"IMPORTED_LINK_INTERFACE_LIBRARIES" "OpenGL"
    "INTERFACE_INCLUDE_DIRECTORIES" "${source_dir}"
)
endif()

