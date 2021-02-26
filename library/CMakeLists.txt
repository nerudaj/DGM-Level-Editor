cmake_minimum_required ( VERSION 3.14 )

# C++ standards
# -------------
include ( "${CMAKE_SOURCE_DIR}/cmake/cpp.cmake" )

# Project
# -------
project ( editor-lib )

# Sources
#--------
set ( SOURCE_FILES
	"src/AppStateEditor.cpp"
	"src/BackgroundGrid.cpp"
	"src/Camera.cpp"
	"src/Editor.cpp"
	"src/EditorState.cpp"
	"src/FileApi.cpp"
	"src/JsonHelper.cpp"
	"src/LogConsole.cpp"
	"src/NewLevelDialog.cpp"
	"src/Tool.cpp"
	"src/ToolProperty.cpp"
	"src/ToolMesh.cpp"
	"src/ToolItem.cpp"
	"src/ToolTrigger.cpp"
	"src/Tilemap.cpp"
)

set ( HEADER_FILES
	"include/AppStateEditor.hpp"
	"include/BackgroundGrid.hpp"
	"include/Camera.hpp"
	"include/Editor.hpp"
	"include/EditorState.hpp"
	"include/FileApi.hpp"
	"include/Globals.hpp"
	"include/JsonHelper.hpp"
	"include/LogConsole.hpp"
	"include/NewLevelDialog.hpp"
	"include/Tool.hpp"
	"include/ToolProperty.hpp"
	"include/ToolMesh.hpp"
	"include/ToolItem.hpp"
	"include/ToolTrigger.hpp"
	"include/Tilemap.hpp"
)

# Target
# ------
add_library ( ${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES} )
target_include_directories ( ${PROJECT_NAME} PUBLIC
	"${SFML_FOLDER}/include"
	"${DGM_FOLDER}/include"
	"${TGUI_FOLDER}/include"
	"${JSON_FOLDER}"
)
target_include_directories( ${PROJECT_NAME} PRIVATE
	"${CMAKE_CURRENT_SOURCE_DIR}/include"
)
target_compile_features( ${PROJECT_NAME} INTERFACE ${MY_CXX_COMPILE_FEATURES} )

# Test
add_subdirectory ( test )