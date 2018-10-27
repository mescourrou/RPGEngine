if (NOT TARGET googlelog)

INCLUDE(ExternalProject)

ExternalProject_Add(
	googlelog
	DOWNLOAD_COMMAND ""
	SOURCE_DIR "${RPG_THIRDPARTY_DIR}/glog"
	PREFIX "${CMAKE_BINARY_DIR}/glog"
	INSTALL_COMMAND ""
	)


endif()
# Get GTest source and binary directories from CMake project
ExternalProject_Get_Property(googlelog source_dir binary_dir)


if (NOT TARGET glog)
# Create a libgtest target to be used as a dependency by test programs
add_library(glog STATIC IMPORTED GLOBAL)
add_dependencies(glog googlelog)

list(APPEND glog_includes "${binary_dir}")
list(APPEND glog_includes "${source_dir}/src")

# Set libgtest properties
set_target_properties(glog PROPERTIES
	"IMPORTED_LOCATION" "${binary_dir}/libglog.a"
	INTERFACE_INCLUDE_DIRECTORIES "${glog_includes}"
)
message("${source_dir}/src/glog/")
endif()
