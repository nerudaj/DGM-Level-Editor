#include "include/Utilities/FontLoader.hpp"


bool FontLoader::loadFonts(
	std::vector<std::filesystem::path> const& paths)
{
	for (auto&& path : paths)
	{
		try
		{
			resmgr.loadResource<sf::Font>(path.string());
			loadedFontName = path.filename().string();
			return true;
		}
		catch (...) {}
	}

	return false;
}