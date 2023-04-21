cmake_minimum_required ( VERSION 3.19 )

file (
	COPY "${CMAKE_BINARY_DIR}/.clang-format"
	DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}"
)

project ( editor-lib )

set ( HEADER_FILES
	"${CMAKE_CURRENT_SOURCE_DIR}/include/AppStateEditor.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Camera.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Globals.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/JsonHelper.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/LogConsole.hpp"
)

set ( SOURCE_FILES
	"${CMAKE_CURRENT_SOURCE_DIR}/src/AppStateEditor.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/Camera.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/JsonHelper.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/LogConsole.cpp"
)

glob_sources_and_headers ( COMMANDS "Commands" )
glob_sources_and_headers ( CONFIGS "Configs" )
glob_sources_and_headers ( DIALOGS "Dialogs" )
glob_sources_and_headers ( EDITOR "Editor" )
glob_sources_and_headers ( INTERFACES "Interfaces" )
glob_sources_and_headers ( LEVELMESH "LevelMesh" )
glob_sources_and_headers ( SHORTCUTS "Shortcuts" )
glob_sources_and_headers ( TOOLS "Tools" )
glob_sources_and_headers ( TOOLPROPS "ToolProperties" )
glob_sources_and_headers ( UTILITIES "Utilities" )

source_group ( "Commands" FILES ${COMMANDS_HEADERS} ${COMMANDS_SOURCES} )
source_group ( "Configs" FILES ${CONFIGS_HEADERS} ${CONFIGS_SOURCES} )
source_group ( "Dialogs" FILES ${DIALOGS_HEADERS} ${DIALOGS_SOURCES} )
source_group ( "Editor" FILES ${EDITOR_HEADERS} ${EDITOR_SOURCES} )
source_group ( "Interfaces" FILES ${INTERFACES_HEADERS} ${INTERFACES_SOURCES} )
source_group ( "Level Mesh" FILES ${LEVELMESH_HEADERS} ${LEVELMESH_SOURCES} )
source_group ( "Shortcuts" FILES ${SHORTCUTS_HEADERS} ${SHORTCUTS_SOURCES} )
source_group ( "Tools" FILES ${TOOLS_HEADERS} ${TOOLS_SOURCES} )
source_group ( "Tool Properties" FILES ${TOOLPROPS_HEADERS} ${TOOLPROPS_SOURCES} )
source_group ( "Utilities" FILES ${UTILITIES_HEADERS} ${UTILITIES_SOURCES} )

add_library ( ${PROJECT_NAME} STATIC
	${HEADER_FILES} ${SOURCE_FILES}
	${COMMANDS_HEADERS} ${COMMANDS_SOURCES}
	${CONFIGS_HEADERS} ${CONFIGS_SOURCES}
	${DIALOGS_HEADERS} ${DIALOGS_SOURCES}
	${EDITOR_HEADERS} ${EDITOR_SOURCES}
	${INTERFACES_HEADERS} ${INTERFACES_SOURCES}
	${LEVELMESH_HEADERS} ${LEVELMESH_SOURCES}
	${SHORTCUTS_HEADERS} ${SHORTCUTS_SOURCES}
	${TOOLS_HEADERS} ${TOOLS_SOURCES}
	${TOOLPROPS_HEADERS} ${TOOLPROPS_SOURCES}
	${UTILITIES_HEADERS} ${UTILITIES_SOURCES}
	"${CMAKE_CURRENT_SOURCE_DIR}/.clang-format"
)

target_include_directories ( ${PROJECT_NAME}
	PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}"
	PUBLIC "${DGM_FOLDER}/include"
	PUBLIC "${DSH_FOLDER}/include"
	PUBLIC "${SFML_FOLDER}/include"
	PUBLIC "${TGUI_FOLDER}/include"
	PUBLIC "${JSON_FOLDER}/single_include/nlohmann"
)

target_link_libraries ( ${PROJECT_NAME}
	${LIB_DGM} ${LIB_SFML} ${LIB_TGUI}
	${LIB_CFG} ${LIB_LVLD}
)

target_compile_features( ${PROJECT_NAME}
	INTERFACE ${MY_CXX_COMPILE_FEATURES}
)

target_compile_options ( ${PROJECT_NAME}
	PRIVATE /MP /analyze:external- /external:I "${JSON_FOLDER}/single_include/nlohmann"
)

target_precompile_headers ( ${PROJECT_NAME}
	PRIVATE
		<vector>
		<format>
		<map>
		<unordered_map>
		<array>
		<optional>
		<expected>
		<fstream>
		<filesystem>
		<functional>
		<concepts>
		<memory>
		<string>
		<cstdint>
		<DGM/dgm.hpp>
		<TGUI/tgui.hpp>
		<json.hpp>
)

set_property(
	TARGET ${PROJECT_NAME}
	PROPERTY VS_USER_PROPS "${CMAKE_CURRENT_SOURCE_DIR}/../props/EnableUseAfterMoveAnalysis.props"
)

if ( NOT ${DISABLE_BENCHMARKS} )
	add_subdirectory ( benchmarks )
endif()

if ( NOT ${DISABLE_TESTING} )
	add_subdirectory ( tests )
endif()