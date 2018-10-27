set(SimpleIni_PATH "${RPG_THIRDPARTY_DIR}/simpleini")

add_library(simpleini INTERFACE )
target_include_directories(simpleini INTERFACE ${SimpleIni_PATH})
