set ( DGM_VERSION  "2.6.1" )
set ( SFML_VERSION "2.5.1" )
set ( TGUI_VERSION "0.8.6" )
set ( JSON_VERSION "3.7.3" )
set ( CATCH_VERSION "2.10.2" )

set ( DGM_URL  "https://github.com/nerudaj/DGM/releases/download/v${DGM_VERSION}/dgm-${DGM_VERSION}-windows-vc16-64-bit.zip" )
set ( SFML_URL "https://github.com/SFML/SFML/releases/download/${SFML_VERSION}/SFML-${SFML_VERSION}-windows-vc15-64-bit.zip" )
set ( TGUI_URL "https://github.com/texus/TGUI/releases/download/v${TGUI_VERSION}/TGUI-${TGUI_VERSION}-vc15-64bit-for-SFML-${SFML_VERSION}.zip" )
set ( JSON_URL "https://github.com/nlohmann/json/releases/download/v${JSON_VERSION}/json.hpp" )
set ( CATCH_URL "https://github.com/catchorg/Catch2/releases/download/v${CATCH_VERSION}/catch.hpp" )

include ( FetchContent )

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

FetchContent_Declare ( catch
	URL                   "${CATCH_URL}"
	DOWNLOAD_NO_PROGRESS  TRUE
	DOWNLOAD_NO_EXTRACT   TRUE
)
FetchContent_GetProperties(catch)
if (NOT catch_POPULATED)
	FetchContent_Populate(catch)
endif()