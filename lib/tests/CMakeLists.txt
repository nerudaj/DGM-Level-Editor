cmake_minimum_required ( VERSION 3.19 )

project ( lib-tests )
copy_dlls_to_build_folders()

# Copy tgui libs as well
configure_file (
	${TGUI_FOLDER}/bin/tgui-d.dll
	${CMAKE_CURRENT_BINARY_DIR}/Debug/ COPYONLY
)

configure_file (
	${TGUI_FOLDER}/bin/tgui.dll
	${CMAKE_CURRENT_BINARY_DIR}/Release/ COPYONLY
)

set ( HEADERS
	"${CMAKE_CURRENT_SOURCE_DIR}/include/FileApiMock.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/EditorMock.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/YesNoCancelDialogMock.hpp"
)

set ( SOURCES
	"${CMAKE_CURRENT_SOURCE_DIR}/src/Main.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/ToolMeshTest.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/AppStateEditor.cpp"
)

add_executable ( ${PROJECT_NAME}
	${SOURCES} ${HEADERS}
) 

target_include_directories ( ${PROJECT_NAME}
	PRIVATE "${CATCH_FOLDER}"
)

target_link_libraries( ${PROJECT_NAME}
	editor-lib
)
