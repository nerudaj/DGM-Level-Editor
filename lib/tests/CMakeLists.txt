cmake_minimum_required ( VERSION 3.19 )

file (
	COPY "${CMAKE_BINARY_DIR}/.clang-format"
	DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}"
)

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

configure_file (
	"${CMAKE_CURRENT_SOURCE_DIR}/include/TestHelpers/TestAssets.hpp.in"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/TestHelpers/TestAssets.hpp"
)

file (
	GLOB HEADERS
	CONFIGURE_DEPENDS
	"${CMAKE_CURRENT_SOURCE_DIR}/include/**/*.hpp"
)

file (
	GLOB SOURCES
	CONFIGURE_DEPENDS
	"${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_executable ( ${PROJECT_NAME}
	${SOURCES} ${HEADERS}
	"${CMAKE_CURRENT_SOURCE_DIR}/.clang-format"
) 

target_include_directories ( ${PROJECT_NAME}
	PRIVATE "${CATCH_FOLDER}"
	PRIVATE "${FAKEIT_FOLDER}/single_header/standalone"
	PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries( ${PROJECT_NAME}
	editor-lib
)

add_test (
	NAME ${PROJECT_NAME}
	COMMAND ${PROJECT_NAME}
)
