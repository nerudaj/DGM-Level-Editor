set ( SETTINGS_PROJECT_NAME "dgm-level-editor-project" )
set ( SETTINGS_BINARY_NAME  "dgm-level-editor" )

if ( "${SETTINGS_PROJECT_NAME}" EQUAL "${SETTINGS_BINARY_NAME}" )
	message ( FATAL_ERROR "SETTINGS_PROJECT_NAME must differ from SETTINGS_BINARY_NAME" )
endif()
