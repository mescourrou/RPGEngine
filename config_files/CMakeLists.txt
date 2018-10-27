add_custom_target(CONFIG_FILE)

include_directories(${CMAKE_CURRENT_BINARY_DIR})


configure_file(config.hpp.in general_config.hpp @ONLY)

