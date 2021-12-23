cmake_minimum_required ( VERSION 3.19 )

project ( "${SETTINGS_BINARY_NAME}" )
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

# Create final executable
add_executable ( ${PROJECT_NAME} WIN32
	Main.cpp
)

target_link_libraries ( ${PROJECT_NAME}
	game-lib
)

set_target_properties ( ${PROJECT_NAME} PROPERTIES
	VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
)

# Packaging
install (
	FILES       "${CMAKE_CURRENT_BINARY_DIR}/Release/${SETTINGS_BINARY_NAME}.exe"
	DESTINATION "bin"
)

file ( GLOB DLLS "${CMAKE_CURRENT_BINARY_DIR}/Release/*.dll" )

install (
	FILES       ${DLLS}
	DESTINATION "bin"
)
