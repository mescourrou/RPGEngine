#if (NOT TARGET ImNodes_project)

#INCLUDE(ExternalProject)

#ExternalProject_Add(
#    ImNodes_project
#    DOWNLOAD_COMMAND ""
#    SOURCE_DIR "${RPG_THIRDPARTY_DIR}/ImNodes"
#    PREFIX "${CMAKE_BINARY_DIR}/ImNodes"
#    CMAKE_ARGS -DIMGUI_DIR=${RPG_THIRDPARTY_DIR}/imgui# -DSFML_DIR=${SFML_BUILD}
#    INSTALL_COMMAND ""
#    )
#endif()

#ExternalProject_Get_Property(ImNodes_project source_dir binary_dir)

#if (NOT TARGET ImNodes)
#    add_library(ImNodes STATIC IMPORTED GLOBAL)
#    add_dependencies(ImNodes ImNodes_project)

#    set(INCLUDED_DIRECTORIES ${source_dir} ${RPG_THIRDPARTY_DIR}/imgui)

#    set_target_properties(ImNodes PROPERTIES
#    "IMPORTED_LOCATION" "${binary_dir}/libImNodes.a"
##    "IMPORTED_LINK_INTERFACE_LIBRARIES" "GL"
#    "INTERFACE_INCLUDE_DIRECTORIES" "${INCLUDED_DIRECTORIES}"
#)
#endif()

find_package(ImGUI-SFML REQUIRED)

if (NOT TARGET ImNodes)
    set(ImNodes_SOURCE_DIR "${RPG_THIRDPARTY_DIR}/ImNodes")
    set(ImNodes_SOURCES
        ${ImNodes_SOURCE_DIR}/ImNodes.h
        ${ImNodes_SOURCE_DIR}/ImNodes.cpp
        ${ImNodes_SOURCE_DIR}/ImNodesEz.h
        ${ImNodes_SOURCE_DIR}/ImNodesEz.cpp)
    add_library(ImNodes STATIC ${ImNodes_SOURCES})
    target_link_libraries(ImNodes ImGui-SFML)

#    set(INCLUDED_DIRECTORIES ${source_dir} ${RPG_THIRDPARTY_DIR}/imgui)

    set_target_properties(ImNodes PROPERTIES
#    "IMPORTED_LOCATION" "${binary_dir}/libImNodes.a"
#    "IMPORTED_LINK_INTERFACE_LIBRARIES" "GL"
    "INTERFACE_INCLUDE_DIRECTORIES" "${ImNodes_SOURCE_DIR}"
)
endif()

