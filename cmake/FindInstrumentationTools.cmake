INCLUDE(ExternalProject)

if (NOT TARGET InstrumentationTools_project)
    ExternalProject_Add(
        InstrumentationTools_project
        DOWNLOAD_COMMAND ""
        SOURCE_DIR "${RPG_THIRDPARTY_DIR}/InstrumentationTools"
        PREFIX "${CMAKE_BINARY_DIR}/InstrumentationTools"
#        CMAKE_ARGS -DBUILD_TEST=OFF -DPROFILING=ON -DBUILD_USE_FILESYSTEM=${BUILD_USE_FILESYSTEM}
        INSTALL_COMMAND ""
        )
endif()


if (NOT TARGET InstrumentationTools)
ExternalProject_Get_Property(InstrumentationTools_project source_dir binary_dir)
add_library(InstrumentationTools SHARED IMPORTED GLOBAL)
add_dependencies(InstrumentationTools InstrumentationTools_project)
message(${binary_dir})
message(${source_dir})
set(INCLUDE_DIRECTORIES "")
list(APPEND INCLUDE_DIRECTORIES "${source_dir}/src")
list(APPEND INCLUDE_DIRECTORIES "${binary_dir}/src")
set_target_properties(InstrumentationTools PROPERTIES
    "IMPORTED_LOCATION" "${binary_dir}/src/libInstrumentationTools.so"
    "INTERFACE_INCLUDE_DIRECTORIES" "${INCLUDE_DIRECTORIES}"
)
endif()
