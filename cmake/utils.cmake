function (copy_dlls_to_build_folders)
	message ( "Copying DLLs to build folders" )
	message ( "${SFML_FOLDER}" )

	file (GLOB DEBUG_SFML_DLLS   ${SFML_FOLDER}/bin/*-d-2.dll)
	file (GLOB RELEASE_SFML_DLLS ${SFML_FOLDER}/bin/*[oskmw]-2.dll)
	set ( OPENAL_DLL 			 ${SFML_FOLDER}/bin/openal32.dll )

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
endfunction (copy_dlls_to_build_folders)

function ( glob_sources_and_headers outvarname subfoldername )

    file (
        GLOB HEADERS
        CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/include/${subfoldername}/*.hpp"
    )
    
    set ( "${outvarname}_HEADERS" "${HEADERS}" PARENT_SCOPE )
    
    file (
        GLOB SOURCES
        CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/${subfoldername}/*.cpp"
    )
    
    set ( "${outvarname}_SOURCES" "${SOURCES}" PARENT_SCOPE )

endfunction ()