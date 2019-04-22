if (NOT TARGET googleflags)

INCLUDE(ExternalProject)

ExternalProject_Add(
	googleflags
	DOWNLOAD_COMMAND ""
	SOURCE_DIR "${RPG_THIRDPARTY_DIR}/gflags"
	PREFIX "${CMAKE_BINARY_DIR}/gflags"
	INSTALL_COMMAND ""
	)


endif()
ExternalProject_Get_Property(googleflags source_dir binary_dir)


if (NOT TARGET gflags)
add_library(gflags STATIC IMPORTED GLOBAL)
add_dependencies(gflags googleflags)

list(APPEND gflags_includes "${binary_dir}/include")

set_target_properties(gflags PROPERTIES
	"IMPORTED_LOCATION" "${binary_dir}/lib/libgflags.a"
	INTERFACE_INCLUDE_DIRECTORIES "${gflags_includes}"
)
endif()
