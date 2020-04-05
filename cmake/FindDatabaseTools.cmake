INCLUDE(ExternalProject)

if (NOT TARGET DatabaseTools_project)
    ExternalProject_Add(
        DatabaseTools_project

        DOWNLOAD_COMMAND ""
        SOURCE_DIR "${RPG_THIRDPARTY_DIR}/DatabaseTools"
        PREFIX "${CMAKE_BINARY_DIR}/DatabaseTools"
        #CMAKE_ARGS -DBUILD_TEST=OFF -DBUILD_USE_FILESYSTEM=${BUILD_USE_FILESYSTEM}
        INSTALL_COMMAND ""
        )
endif()
ExternalProject_Get_Property(DatabaseTools_project source_dir binary_dir)


if (NOT TARGET DatabaseTools)
add_library(DatabaseTools STATIC IMPORTED GLOBAL)
add_dependencies(DatabaseTools DatabaseTools_project)

set_target_properties(DatabaseTools PROPERTIES
    "IMPORTED_LOCATION" "${binary_dir}/libDatabaseTools.a"
    INTERFACE_LINK_LIBRARIES sqlite
    "INTERFACE_INCLUDE_DIRECTORIES" "${source_dir}/src"
)
endif()
