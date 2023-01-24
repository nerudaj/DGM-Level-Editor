cmake_minimum_required ( VERSION 3.19 )

include ( "${CMAKE_SOURCE_DIR}/cmake/settings.cmake" )

project ( "${SETTINGS_PROJECT_NAME}" )
include ( "${CMAKE_SOURCE_DIR}/cmake/cpp.cmake" )
include ( "${CMAKE_SOURCE_DIR}/cmake/version.cmake" )
include ( "${CMAKE_SOURCE_DIR}/cmake/utils.cmake" )

option ( DISABLE_TESTING    "Disable unit test projects" OFF )
option ( DISABLE_BENCHMARKS "Disable benchmarking projects" ON )
option ( CACHE_THIRD_PARTY  "Downloaded dependencies will be stored outside of build dir" ON )

if ( ${CACHE_THIRD_PARTY} )
	set ( FETCHCONTENT_BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/deps" )
endif()

if ( NOT ${DISABLE_TESTING})
	enable_testing()
endif()

include ( "${CMAKE_SOURCE_DIR}/cmake/dependencies.cmake" )

configure_file(
	"${CMAKE_SOURCE_DIR}/props/EnableUseAfterMoveAnalysis.props.in"
	"${CMAKE_SOURCE_DIR}/props/EnableUseAfterMoveAnalysis.props"
)

add_subdirectory ( lib )
add_subdirectory ( bin )

# Packaging
install (
	DIRECTORY   "resources"
	DESTINATION "."
)

install (
	DIRECTORY   "sample_project"
	DESTINATION "."
)

install (
	FILES       "changelog.txt"
	DESTINATION "."
)

message ( "ProjectVersion is ${PROJECT_VERSION}")

set ( CPACK_GENERATOR "ZIP" )
set ( CPACK_PACKAGE_VERSION_MAJOR       "${PROJECT_VERSION_MAJOR}" )
set ( CPACK_PACKAGE_VERSION_MINOR       "${PROJECT_VERSION_MINOR}" )
set ( CPACK_PACKAGE_VERSION_PATCH       "${PROJECT_VERSION_PATCH}" )
set ( CPACK_PACKAGE_FILE_NAME           "${PROJECT_NAME}-${PROJECT_VERSION}-windows-${CMAKE_GENERATOR_PLATFORM}" )

include ( CPack )
