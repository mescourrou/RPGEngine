find_package(Threads REQUIRED)

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
    if (NOT EXISTS ${binary_dir}/include)
        file(MAKE_DIRECTORY ${binary_dir}/include)
    endif()
add_library(gflags STATIC IMPORTED GLOBAL)
add_dependencies(gflags googleflags)

set_target_properties(gflags PROPERTIES
    "IMPORTED_LOCATION" "${binary_dir}/lib/libgflags.a"
    "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
    "INTERFACE_INCLUDE_DIRECTORIES" "${binary_dir}/include"
)
endif()
