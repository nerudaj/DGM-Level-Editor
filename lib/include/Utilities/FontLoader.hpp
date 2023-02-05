#pragma once

#include <DGM/dgm.hpp>

#include <filesystem>
#include <vector>
#include <string>

class FontLoader
{
public:
	FontLoader(dgm::ResourceManager& resmgr)
		: resmgr(resmgr)
	{}

public:
	/**
	 *  \brief Tries to load fonts. First successully loaded font
	 *  will be present in resource manager.
	 *
	 *  \returns TRUE if a font was loaded.
	 *
	 *  \post Use getLoadedFontName to retrieve ID of loaded font
	 */
	bool loadFonts(std::vector<std::filesystem::path> const& paths);

	constexpr std::string const& getLoadedFontName() const noexcept
	{
		return loadedFontName;
	}

protected:
	dgm::ResourceManager& resmgr;
	std::string loadedFontName;
};
