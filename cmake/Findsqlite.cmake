if (NOT TARGET sqlite_project)

INCLUDE(ExternalProject)

ExternalProject_Add(
	sqlite_project
	DOWNLOAD_COMMAND ""
	SOURCE_DIR "${RPG_THIRDPARTY_DIR}/sqlite3"
	PREFIX "${CMAKE_BINARY_DIR}/sqlite3"
	INSTALL_COMMAND ""
	)


endif()
# Get GTest source and binary directories from CMake project
ExternalProject_Get_Property(sqlite_project source_dir binary_dir)


if (NOT TARGET sqlite)
# Create a libgtest target to be used as a dependency by test programs
add_library(sqlite SHARED IMPORTED GLOBAL)
add_dependencies(sqlite sqlite_project)

list(APPEND sqlite_includes "${source_dir}")

# Set libgtest properties
set_target_properties(sqlite PROPERTIES
	"IMPORTED_LOCATION" "${binary_dir}/libsqlite3.so"
	INTERFACE_INCLUDE_DIRECTORIES "${sqlite_includes}"
)
endif()
