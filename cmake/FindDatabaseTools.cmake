INCLUDE(ExternalProject)

if (NOT TARGET DatabaseTools_project)
    ExternalProject_Add(
        DatabaseTools_project

        DOWNLOAD_COMMAND ""
        SOURCE_DIR "${RPG_THIRDPARTY_DIR}/DatabaseTools"
        PREFIX "${CMAKE_BINARY_DIR}/DatabaseTools"
        CMAKE_ARGS -DBUILD_TEST=OFF -DBUILD_USE_FILESYSTEM=${BUILD_USE_FILESYSTEM}
        INSTALL_COMMAND ""
        )
endif()


if (NOT TARGET DatabaseTools)
ExternalProject_Get_Property(DatabaseTools_project source_dir binary_dir)
add_library(DatabaseTools SHARED IMPORTED GLOBAL)
add_dependencies(DatabaseTools DatabaseTools_project)
set_target_properties(DatabaseTools PROPERTIES
    "IMPORTED_LOCATION" "${binary_dir}/libDatabaseTools.so"
    "INTERFACE_INCLUDE_DIRECTORIES" "${source_dir}/src"
)
endif()
