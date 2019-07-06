if (NOT TARGET json)
add_library(json INTERFACE)
target_include_directories(json INTERFACE "${RPG_THIRDPARTY_DIR}/json/single_include/nlohmann")
endif()
