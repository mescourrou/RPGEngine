if (NOT TARGET ImGui)
add_library(ImGui
	${RPG_THIRDPARTY_DIR}/imgui/imconfig.h
	${RPG_THIRDPARTY_DIR}/imgui/imgui.cpp
	${RPG_THIRDPARTY_DIR}/imgui/imgui.h
	${RPG_THIRDPARTY_DIR}/imgui/imgui_internal.h
	${RPG_THIRDPARTY_DIR}/imgui/imgui_widgets.cpp
	${RPG_THIRDPARTY_DIR}/imgui/imgui_draw.cpp
	${RPG_THIRDPARTY_DIR}/imgui/imstb_rectpack.h
	${RPG_THIRDPARTY_DIR}/imgui/imstb_textedit.h
	${RPG_THIRDPARTY_DIR}/imgui/imstb_truetype.h
	)

set_target_properties(ImGui PROPERTIES
	"INTERFACE_INCLUDE_DIRECTORIES" "${RPG_THIRDPARTY_DIR}/imgui/")
endif()

