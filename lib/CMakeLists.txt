cmake_minimum_required ( VERSION 3.19 )

project ( editor-lib )

set ( HEADER_FILES
	"${CMAKE_CURRENT_SOURCE_DIR}/include/AppStateEditor.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/BackgroundGrid.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Camera.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Editor.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/EditorState.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/FileApi.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Globals.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/JsonHelper.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/LogConsole.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Tilemap.hpp"
)

set ( SOURCE_FILES
	"${CMAKE_CURRENT_SOURCE_DIR}/src/AppStateEditor.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/BackgroundGrid.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/Camera.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/Editor.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/EditorState.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/FileApi.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/JsonHelper.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/LogConsole.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/Tilemap.cpp"
)

set ( CONFIGS_HEADER_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/include/Configs" )
set ( CONFIGS_SOURCE_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/src/Configs" )

set ( CONFIGS_HEADERS
	"${CONFIGS_HEADER_PREFIX}/Sizers.hpp"
)

set ( CONFIGS_SOURCES
	"${CONFIGS_SOURCE_PREFIX}/Sizers.cpp"
)

set ( TOOLS_HEADER_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/include/Tools" )
set ( TOOLS_SOURCE_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/src/Tools" )

set ( TOOLS_HEADERS
	"${TOOLS_HEADER_PREFIX}/Tool.hpp"
	"${TOOLS_HEADER_PREFIX}/ToolItem.hpp"
	"${TOOLS_HEADER_PREFIX}/ToolMesh.hpp"
	"${TOOLS_HEADER_PREFIX}/ToolProperty.hpp"
	"${TOOLS_HEADER_PREFIX}/ToolTrigger.hpp"
)

set ( TOOLS_SOURCES
	"${TOOLS_SOURCE_PREFIX}/Tool.cpp"
	"${TOOLS_SOURCE_PREFIX}/ToolItem.cpp"
	"${TOOLS_SOURCE_PREFIX}/ToolMesh.cpp"
	"${TOOLS_SOURCE_PREFIX}/ToolProperty.cpp"
	"${TOOLS_SOURCE_PREFIX}/ToolTrigger.cpp"
)

set ( DIALOGS_HEADER_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/include/Dialogs" )
set ( DIALOGS_SOURCE_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/src/Dialogs" )

set ( DIALOGS_HEADERS
	"${DIALOGS_HEADER_PREFIX}/DialogBase.hpp"
	"${DIALOGS_HEADER_PREFIX}/NewLevelDialog.hpp"
	"${DIALOGS_HEADER_PREFIX}/ResizeLevelDialog.hpp"
	"${DIALOGS_HEADER_PREFIX}/YesNoCancelDialog.hpp"
)

set ( DIALOGS_SOURCES
	"${DIALOGS_SOURCE_PREFIX}/DialogBase.cpp"
	"${DIALOGS_SOURCE_PREFIX}/NewLevelDialog.cpp"
	"${DIALOGS_SOURCE_PREFIX}/ResizeLevelDialog.cpp"
	"${DIALOGS_SOURCE_PREFIX}/YesNoCancelDialog.cpp"
)

source_group ( "Configs" FILES ${CONFIGS_HEADERS} ${CONFIGS_SOURCES} )
source_group ( "Tools" FILES ${TOOLS_HEADERS} ${TOOLS_SOURCES} )
source_group ( "Dialogs" FILES ${DIALOGS_HEADERS} ${DIALOGS_SOURCES} )

add_library ( ${PROJECT_NAME} STATIC
	${HEADER_FILES} ${SOURCE_FILES}
	${CONFIGS_HEADERS} ${CONFIGS_SOURCES}
	${TOOLS_HEADERS} ${TOOLS_SOURCES}
	${DIALOGS_HEADERS} ${DIALOGS_SOURCES}
	"${CMAKE_CURRENT_SOURCE_DIR}/.editorconfig"
)

target_include_directories ( ${PROJECT_NAME}
	PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}"
	PUBLIC "${DGM_FOLDER}/include/include"
	PUBLIC "${DGM_FOLDER}/include"
	PUBLIC "${SFML_FOLDER}/include"
	PUBLIC "${TGUI_FOLDER}/include"
	PUBLIC "${JSON_FOLDER}/single_include/nlohmann"
)

target_link_libraries ( ${PROJECT_NAME}
	${LIB_DGM} ${LIB_SFML} ${LIB_TGUI}
)

target_compile_features( ${PROJECT_NAME}
	INTERFACE ${MY_CXX_COMPILE_FEATURES}
)

target_compile_options ( ${PROJECT_NAME}
	PRIVATE /MP
)

add_subdirectory ( benchmarks )
add_subdirectory ( tests )
