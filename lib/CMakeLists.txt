cmake_minimum_required ( VERSION 3.19 )

project ( game-lib )

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
	"${CMAKE_CURRENT_SOURCE_DIR}/include/NewLevelDialog.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Tilemap.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Tool.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ToolItem.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ToolMesh.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ToolProperty.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/ToolTrigger.hpp"
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
	"${CMAKE_CURRENT_SOURCE_DIR}/src/NewLevelDialog.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/Tilemap.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/Tool.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/ToolItem.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/ToolMesh.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/ToolProperty.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/ToolTrigger.cpp"
)

add_library ( ${PROJECT_NAME} STATIC
	${HEADER_FILES} ${SOURCE_FILES}
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
