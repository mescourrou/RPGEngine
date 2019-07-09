find_package(SFML REQUIRED)

add_library(BaseGUIObject
    BaseGUIObject.cpp BaseGUIObject.hpp)
target_link_libraries(BaseGUIObject ${SFML_TARGETS})

target_include_directories(BaseGUIObject PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(BaseGUIObject PUBLIC "${CMAKE_BINARY_DIR}/${RPG_CONFIG_FILE_DIR}")
