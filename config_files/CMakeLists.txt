add_custom_target(CONFIG_FILE)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

configure_file(compiler_proper_defines.hpp compiler_proper_defines.hpp)
configure_file(config.hpp.in general_config.hpp @ONLY)

