cmake_minimum_required ( VERSION 3.10 )

## Project setup
project ( dgm-level-editor
	LANGUAGES CXX
)

## C++ standards
set ( CMAKE_CXX_STANDARD             17 )
set ( CMAKE_CXX_STANDARD_REQUIRED    ON )
set ( CMAKE_CXX_EXTENSIONS           OFF )
set ( MY_CXX_COMPILE_FEATURES        cxx_generic_lambdas cxx_range_for cxx_strong_enums )
set ( CMAKE_GENERATOR_PLATFORM       x64)
set ( VS_DEBUGGER_COMMAND_ARGUMENTS  "../.." )

## Dependencies
set ( SFML_FOLDER  "${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/SFML" )
set ( DGM_FOLDER   "${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/DGM" )
set ( TGUI_FOLDER  "${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/TGUI" )
set ( JSON_FOLDER  "${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/json" )

include_directories(
	"${SFML_FOLDER}/include"
	"${DGM_FOLDER}/include"
	"${TGUI_FOLDER}/include"
	"${JSON_FOLDER}"
)

link_directories(
	"${SFML_FOLDER}/lib"
	"${DGM_FOLDER}/lib"
	"${TGUI_FOLDER}/lib"
)

### Find libraries
# NOTE: SFML is linked dynamically from DGM
find_library ( LIB_DGM_D dgm-d
	NAMES
		dgm-d.lib
	HINTS
		"${DGM_FOLDER}/lib"
)

find_library ( LIB_DGM_R dgm
	NAMES
		dgm.lib
	HINTS
		"${DGM_FOLDER}/lib"
)

set ( LIB_DGM
	optimized
		${LIB_DGM_R}
	debug
		${LIB_DGM_D}
)

find_library ( LIB_TGUI_D tgui-d 
	NAMES
		tgui-d.lib
	HINTS
		"${TGUI_FOLDER}/lib"
)

find_library ( LIB_TGUI_R tgui 
	NAMES
		tgui.lib
	HINTS
		"${TGUI_FOLDER}/lib"
)

set ( LIB_TGUI
	optimized
		${LIB_TGUI_R}
	debug
		${LIB_TGUI_D}
)

set ( SOURCE_FILES
	src/Main.cpp
	src/AppStateMainMenu.cpp
	src/AppStateMenuOptions.cpp
	src/AppStateIngame.cpp
	src/AppStateEditor.cpp
	src/Editor.cpp
	src/EditorLayer.cpp
	src/EditorBrush.cpp
	src/Itemset.cpp
	src/FileApi.cpp
)

set ( HEADER_FILES
	src/Globals.hpp
	src/AppStateMainMenu.hpp
	src/AppStateMenuOptions.hpp
	src/AppStateIngame.hpp
	src/AppStateEditor.hpp
	src/Editor.hpp
	src/EditorHistory.hpp
	src/EditorLayer.hpp
	src/EditorMode.hpp
	src/EditorBrush.hpp
	src/Itemset.hpp
	src/FileApi.hpp
)

add_executable ( ${PROJECT_NAME} WIN32
	${SOURCE_FILES}
	${HEADER_FILES}
)
target_link_libraries( ${PROJECT_NAME} ${LIB_DGM} ${LIB_TGUI} )
target_compile_features( ${PROJECT_NAME} INTERFACE ${MY_CXX_COMPILE_FEATURES} )