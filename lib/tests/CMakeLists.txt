cmake_minimum_required ( VERSION 3.19 )

project ( lib-tests )

set ( HEADERS
	"${CMAKE_CURRENT_SOURCE_DIR}/include/FileApiMock.hpp"
)

set ( SOURCES
	"${CMAKE_CURRENT_SOURCE_DIR}/src/Main.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/ToolMeshTest.cpp"
)

add_executable ( ${PROJECT_NAME}
	${SOURCES}
) 

target_include_directories ( ${PROJECT_NAME}
	PRIVATE "${CATCH_FOLDER}"
)

target_link_libraries( ${PROJECT_NAME}
	editor-lib
)
