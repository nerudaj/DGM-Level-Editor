cmake_minimum_required ( VERSION 3.19 )

project ( editor-lib )

set ( HEADER_FILES
	"${CMAKE_CURRENT_SOURCE_DIR}/include/AppStateEditor.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Camera.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Editor.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/EditorState.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/FileApi.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Globals.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/JsonHelper.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/LogConsole.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ShortcutCombo.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ShortcutEngineInterface.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ShortcutEngine.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Utilities.hpp"
)

set ( SOURCE_FILES
	"${CMAKE_CURRENT_SOURCE_DIR}/src/AppStateEditor.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/Camera.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/Editor.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/EditorState.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/FileApi.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/JsonHelper.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/LogConsole.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/ShortcutCombo.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/ShortcutEngine.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/Utilities.cpp"
)

file (
	GLOB CONFIGS_HEADERS
	CONFIGURE_DEPENDS
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Configs/*.hpp"
)

file (
	GLOB CONFIGS_SOURCES
	CONFIGURE_DEPENDS
	"${CMAKE_CURRENT_SOURCE_DIR}/src/Configs/*.cpp"
)

file (
	GLOB TOOLS_HEADERS
	CONFIGURE_DEPENDS
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Tools/*.hpp"
)

file (
	GLOB TOOLS_SOURCES
	CONFIGURE_DEPENDS
	"${CMAKE_CURRENT_SOURCE_DIR}/src/Tools/*.cpp"
)

file (
	GLOB DIALOGS_HEADERS
	CONFIGURE_DEPENDS
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Dialogs/*.hpp"
)

file (
	GLOB DIALOGS_SOURCES
	CONFIGURE_DEPENDS
	"${CMAKE_CURRENT_SOURCE_DIR}/src/Dialogs/*.cpp"
)

file (
	GLOB LEVELMESH_HEADERS
	CONFIGURE_DEPENDS
	"${CMAKE_CURRENT_SOURCE_DIR}/include/LevelMesh/*.hpp"
)

file (
	GLOB LEVELMESH_SOURCES
	CONFIGURE_DEPENDS
	"${CMAKE_CURRENT_SOURCE_DIR}/src/LevelMesh/*.cpp"
)

file (
	GLOB COMMANDS_HEADERS
	CONFIGURE_DEPENDS
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Commands/*.hpp"
)

file (
	GLOB COMMANDS_SOURCES
	CONFIGURE_DEPENDS
	"${CMAKE_CURRENT_SOURCE_DIR}/src/Commands/*.cpp"
)

source_group ( "Configs" FILES ${CONFIGS_HEADERS} ${CONFIGS_SOURCES} )
source_group ( "Tools" FILES ${TOOLS_HEADERS} ${TOOLS_SOURCES} )
source_group ( "Dialogs" FILES ${DIALOGS_HEADERS} ${DIALOGS_SOURCES} )
source_group ( "Level Mesh" FILES ${LEVELMESH_HEADERS} ${LEVELMESH_SOURCES} )
source_group ( "Commands" FILES ${COMMANDS_HEADERS} ${COMMANDS_SOURCES} )

add_library ( ${PROJECT_NAME} STATIC
	${HEADER_FILES} ${SOURCE_FILES}
	${CONFIGS_HEADERS} ${CONFIGS_SOURCES}
	${TOOLS_HEADERS} ${TOOLS_SOURCES}
	${DIALOGS_HEADERS} ${DIALOGS_SOURCES}
	${LEVELMESH_HEADERS} ${LEVELMESH_SOURCES}
	${COMMANDS_HEADERS} ${COMMANDS_SOURCES}
	"${CMAKE_CURRENT_SOURCE_DIR}/.editorconfig"
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
	PRIVATE /MP
)

if ( NOT ${DISABLE_BENCHMARKS} )
	add_subdirectory ( benchmarks )
endif()

if ( NOT ${DISABLE_TESTING} )
	add_subdirectory ( tests )
endif()