find_library(lua REQUIRED)

if (NOT TARGET LuaBridge)
add_library(LuaBridge INTERFACE)
target_include_directories(LuaBridge INTERFACE "${RPG_THIRDPARTY_DIR}/LuaBridge/Source")
endif()
