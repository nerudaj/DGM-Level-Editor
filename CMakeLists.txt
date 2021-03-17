cmake_minimum_required ( VERSION 3.10 )

# C++ standards
#----------------------------------------------------------
include ( cmake/cpp.cmake )

# Project setup
#----------------------------------------------------------
project ( dgm-level-editor
	LANGUAGES CXX
)

enable_testing()

include ( cmake/version.cmake )

# Add dependencies
#----------------------------------------------------------
include ( cmake/dependencies.cmake )

## Set paths to dependencies
##---------------------------------------------------------
set ( SFML_FOLDER  "${sfml_SOURCE_DIR}" )
set ( DGM_FOLDER   "${dgm_SOURCE_DIR}" )
set ( TGUI_FOLDER  "${tgui_SOURCE_DIR}" )
set ( JSON_FOLDER "${CMAKE_CURRENT_BINARY_DIR}/_deps/json-subbuild/json-populate-prefix/src")
set ( CATCH_FOLDER "${CMAKE_CURRENT_BINARY_DIR}/_deps/catch-src")

## Find libraries
##---------------------------------------------------------
# NOTE: SFML is linked dynamically from DGM
find_library ( LIB_DGM_D dgm-d NAMES dgm-d.lib HINTS "${DGM_FOLDER}/lib" )
find_library ( LIB_DGM_R dgm   NAMES dgm.lib   HINTS "${DGM_FOLDER}/lib" )

set ( LIB_DGM optimized ${LIB_DGM_R} debug ${LIB_DGM_D} )

find_library ( LIB_TGUI_D tgui-d NAMES tgui-d.lib HINTS "${TGUI_FOLDER}/lib" )
find_library ( LIB_TGUI_R tgui   NAMES tgui.lib   HINTS "${TGUI_FOLDER}/lib" )

set ( LIB_TGUI optimized ${LIB_TGUI_R} debug ${LIB_TGUI_D} )

add_subdirectory ( library )

# Sources
#--------
set ( SOURCE_FILES
	"sources/Main.cpp"
)

# Target
# ------
add_executable ( ${PROJECT_NAME} WIN32 ${SOURCE_FILES} )

target_include_directories ( ${PROJECT_NAME} PRIVATE
	"${SFML_FOLDER}/include"
	"${DGM_FOLDER}/include"
	"${TGUI_FOLDER}/include"
	"${JSON_FOLDER}"
	"library/include"
)

target_link_directories ( ${PROJECT_NAME} PRIVATE 
	"${SFML_FOLDER}/lib"
	"${DGM_FOLDER}/lib"
	"${TGUI_FOLDER}/lib"
)

target_link_libraries ( ${PROJECT_NAME} editor-lib ${LIB_DGM} ${LIB_TGUI} )
target_compile_features( ${PROJECT_NAME} INTERFACE ${MY_CXX_COMPILE_FEATURES} )

### Copy DLLs to build folders
###--------------------------------------------------------
file (GLOB DEBUG_SFML_DLLS   ${CMAKE_BINARY_DIR}/_deps/sfml-src/bin/*-d-2.dll)
file (GLOB RELEASE_SFML_DLLS ${CMAKE_BINARY_DIR}/_deps/sfml-src/bin/*[oskmw]-2.dll)
set ( OPENAL_DLL ${CMAKE_BINARY_DIR}/_deps/sfml-src/bin/openal32.dll )

set ( DEBUG_FOLDER "${CMAKE_CURRENT_BINARY_DIR}/Debug" )
set ( RELEASE_FOLDER "${CMAKE_CURRENT_BINARY_DIR}/Release" )

file(MAKE_DIRECTORY ${DEBUG_FOLDER} ${RELEASE_FOLDER} )

configure_file (
	${OPENAL_DLL} ${DEBUG_FOLDER} COPYONLY
)

configure_file (
	${OPENAL_DLL} ${RELEASE_FOLDER} COPYONLY
)

foreach ( DLL ${DEBUG_SFML_DLLS} )
	configure_file ( ${DLL} ${DEBUG_FOLDER} COPYONLY )
endforeach ( DLL )

foreach ( DLL ${RELEASE_SFML_DLLS} )
	configure_file ( ${DLL} ${RELEASE_FOLDER} COPYONLY )
endforeach ( DLL )

configure_file (
	${CMAKE_CURRENT_BINARY_DIR}/_deps/tgui-src/bin/tgui-d.dll
	${DEBUG_FOLDER} COPYONLY
)

configure_file (
	${CMAKE_CURRENT_BINARY_DIR}/_deps/tgui-src/bin/tgui.dll
	${RELEASE_FOLDER} COPYONLY
)

# Installation
#----------------------------------------------------------
install (
    DIRECTORY    resources
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