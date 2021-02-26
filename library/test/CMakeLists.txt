cmake_minimum_required ( VERSION 3.14 )

include ( "../../cmake/cpp.cmake" )

project ( editor-lib-tests )

set ( SOURCES
	"Main.cpp"
	"ToolMeshTest.cpp"
)

add_executable ( ${PROJECT_NAME} ${SOURCES} )

target_include_directories ( ${PROJECT_NAME} PRIVATE
	"${SFML_FOLDER}/include"
	"${DGM_FOLDER}/include"
	"${TGUI_FOLDER}/include"
	"${JSON_FOLDER}"
	"${CATCH_FOLDER}"
	"../include"
)

target_link_directories ( ${PROJECT_NAME} PRIVATE 
	"${SFML_FOLDER}/lib"
	"${DGM_FOLDER}/lib"
	"${TGUI_FOLDER}/lib"
)

target_link_libraries ( ${PROJECT_NAME} editor-lib ${LIB_DGM} ${LIB_TGUI} )
target_compile_features( ${PROJECT_NAME} INTERFACE ${MY_CXX_COMPILE_FEATURES} )

add_test ( NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME} )

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
	${CMAKE_BINARY_DIR}/_deps/tgui-src/bin/tgui-d.dll
	${DEBUG_FOLDER} COPYONLY
)

configure_file (
	${CMAKE_BINARY_DIR}/_deps/tgui-src/bin/tgui.dll
	${RELEASE_FOLDER} COPYONLY
)