cmake_minimum_required ( VERSION 3.10 )

# C++ standards
#----------------------------------------------------------
set ( CMAKE_GENERATOR_PLATFORM     x64)
set ( CMAKE_CXX_STANDARD           17 )
set ( CMAKE_CXX_STANDARD_REQUIRED  ON )
set ( CMAKE_CXX_EXTENSIONS         OFF )
set ( MY_CXX_COMPILE_FEATURES      cxx_generic_lambdas cxx_range_for cxx_strong_enums )

# Project setup
#----------------------------------------------------------
project ( dgm-level-editor
	LANGUAGES CXX
)

include ( version.cmake )

# Dependencies
#----------------------------------------------------------
include ( FetchContent )
include ( dependencies.cmake )

## Add dependencies
##---------------------------------------------------------
set ( DGM_URL  "https://github.com/nerudaj/DGM/releases/download/v${DGM_VERSION}/dgm-${DGM_VERSION}-windows-vc16-64-bit.zip" )
set ( SFML_URL "https://github.com/SFML/SFML/releases/download/${SFML_VERSION}/SFML-${SFML_VERSION}-windows-vc15-64-bit.zip" )
set ( TGUI_URL "https://github.com/texus/TGUI/releases/download/v${TGUI_VERSION}/TGUI-${TGUI_VERSION}-vc15-64bit-for-SFML-${SFML_VERSION}.zip" )
set ( JSON_URL "https://github.com/nlohmann/json/releases/download/v${JSON_VERSION}/json.hpp" )

FetchContent_Declare ( sfml
	URL "${SFML_URL}"
)
FetchContent_GetProperties(sfml)
if (NOT sfml_POPULATED)
	FetchContent_Populate(sfml)
endif()

FetchContent_Declare ( dgm
	URL "${DGM_URL}"
)
FetchContent_GetProperties(dgm)
if (NOT dgm_POPULATED)
	FetchContent_Populate(dgm)
endif()

FetchContent_Declare ( tgui
	URL "${TGUI_URL}"
)
FetchContent_GetProperties(tgui)
if (NOT tgui_POPULATED)
	FetchContent_Populate(tgui)
endif()

FetchContent_Declare ( json
	URL                   "${JSON_URL}"
	DOWNLOAD_NO_PROGRESS  TRUE
	DOWNLOAD_NO_EXTRACT   TRUE
)
FetchContent_GetProperties(json)
if (NOT json_POPULATED)
	FetchContent_Populate(json)
endif()

## Set paths to dependencies
##---------------------------------------------------------
set ( SFML_FOLDER  "${sfml_SOURCE_DIR}" )
set ( DGM_FOLDER   "${dgm_SOURCE_DIR}" )
set ( TGUI_FOLDER  "${tgui_SOURCE_DIR}" )
set (JSON_FOLDER "${CMAKE_CURRENT_BINARY_DIR}/_deps/json-subbuild/json-populate-prefix/src")

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

## Find libraries
##---------------------------------------------------------
# NOTE: SFML is linked dynamically from DGM
find_library ( LIB_DGM_D dgm-d NAMES dgm-d.lib HINTS "${DGM_FOLDER}/lib" )
find_library ( LIB_DGM_R dgm   NAMES dgm.lib   HINTS "${DGM_FOLDER}/lib" )

set ( LIB_DGM optimized ${LIB_DGM_R} debug ${LIB_DGM_D} )

find_library ( LIB_TGUI_D tgui-d NAMES tgui-d.lib HINTS "${TGUI_FOLDER}/lib" )
find_library ( LIB_TGUI_R tgui   NAMES tgui.lib   HINTS "${TGUI_FOLDER}/lib" )

set ( LIB_TGUI optimized ${LIB_TGUI_R} debug ${LIB_TGUI_D} )


### Copy DLLs to build folders
###--------------------------------------------------------
file (GLOB DEBUG_SFML_DLLS   ${CMAKE_CURRENT_BINARY_DIR}/_deps/sfml-src/bin/*-d-2.dll)
file (GLOB RELEASE_SFML_DLLS ${CMAKE_CURRENT_BINARY_DIR}/_deps/sfml-src/bin/*[oskmw]-2.dll)
set ( OPENAL_DLL ${CMAKE_CURRENT_BINARY_DIR}/_deps/sfml-src/bin/openal32.dll )

file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/Debug" "${CMAKE_CURRENT_BINARY_DIR}/Release" )

configure_file (
	${OPENAL_DLL} ${CMAKE_CURRENT_BINARY_DIR}/Debug/ COPYONLY
)

configure_file (
	${OPENAL_DLL} ${CMAKE_CURRENT_BINARY_DIR}/Release/ COPYONLY
)

foreach ( DLL ${DEBUG_SFML_DLLS} )
	configure_file ( ${DLL} ${CMAKE_CURRENT_BINARY_DIR}/Debug/ COPYONLY )
endforeach ( DLL )

foreach ( DLL ${RELEASE_SFML_DLLS} )
	configure_file ( ${DLL} ${CMAKE_CURRENT_BINARY_DIR}/Release/ COPYONLY )
endforeach ( DLL )

configure_file (
	${CMAKE_CURRENT_BINARY_DIR}/_deps/tgui-src/bin/tgui-d.dll
	${CMAKE_CURRENT_BINARY_DIR}/Debug/ COPYONLY
)

configure_file (
	${CMAKE_CURRENT_BINARY_DIR}/_deps/tgui-src/bin/tgui.dll
	${CMAKE_CURRENT_BINARY_DIR}/Release/ COPYONLY
)

# Sources
#----------------------------------------------------------
set ( SOURCE_FILES
	src/AppStateEditor.cpp
	src/BackgroundGrid.cpp
	src/Camera.cpp
	src/Editor.cpp
	src/EditorState.cpp
	src/FileApi.cpp
	src/JsonHelper.cpp
	src/LogConsole.cpp
	src/Main.cpp
	src/NewLevelDialog.cpp
	src/Tool.cpp
	src/ToolProperty.cpp
	src/ToolMesh.cpp
	src/ToolItem.cpp
	src/ToolTrigger.cpp
	src/Tilemap.cpp
)

set ( HEADER_FILES
	src/AppStateEditor.hpp
	src/BackgroundGrid.hpp
	src/Camera.hpp
	src/Editor.hpp
	src/EditorState.hpp
	src/FileApi.hpp
	src/Globals.hpp
	src/JsonHelper.hpp
	src/LogConsole.hpp
	src/NewLevelDialog.hpp
	src/Tool.hpp
	src/ToolProperty.hpp
	src/ToolMesh.hpp
	src/ToolItem.hpp
	src/ToolTrigger.hpp
	src/Tilemap.hpp
)

add_executable ( ${PROJECT_NAME} WIN32
	${SOURCE_FILES}
	${HEADER_FILES}
)
target_link_libraries( ${PROJECT_NAME} ${LIB_DGM} ${LIB_TGUI} )
target_compile_features( ${PROJECT_NAME} INTERFACE ${MY_CXX_COMPILE_FEATURES} )

# Installation
#----------------------------------------------------------
install (
    DIRECTORY    audio
    DESTINATION  .
)

install (
    DIRECTORY    graphics
    DESTINATION  .
)

install (
    FILES        ${CMAKE_CURRENT_BINARY_DIR}/Release/${PROJECT_NAME}.exe
    DESTINATION  bin
)

install (
	FILES        ${RELEASE_SFML_DLLS}
	DESTINATION  bin
)

install (
	FILES        ${OPENAL_DLL}
	DESTINATION  bin
)

install (
	FILES        ${CMAKE_CURRENT_BINARY_DIR}/_deps/tgui-src/bin/tgui.dll
	DESTINATION  bin
)

# Packaging
#----------------------------------------------------------
set ( CPACK_GENERATOR "ZIP" )

set ( CPACK_PACKAGE_DESCRIPTION_SUMMARY "C++ extension library for SFML" )
set ( CPACK_PACKAGE_VERSION_MAJOR       "${PROJECT_VERSION_MAJOR}" )
set ( CPACK_PACKAGE_VERSION_MINOR       "${PROJECT_VERSION_MINOR}" )
set ( CPACK_PACKAGE_VERSION_PATCH       "${PROJECT_VERSION_PATCH}" )
set ( CPACK_PACKAGE_FILE_NAME           "${PROJECT_NAME}-${PROJECT_VERSION}-vc15x64" )

include ( CPack )