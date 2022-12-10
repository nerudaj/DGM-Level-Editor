#include <DGM/dgm.hpp>
#include <Config.hpp>

#include "include/AppStateEditor.hpp"
#include "include/Utilities/FileApi.hpp"
#include "include/Shortcuts/ShortcutEngine.hpp"

int main(int argc, char* argv[])
{
	std::string rootDir = "..";
	if (argc == 2)
	{
		rootDir = argv[1];
	}

	auto fileApi = std::make_unique<FileApi>();
	const auto APPDATA = fileApi->resolveAppdata();

	cfg::Ini ini;
	try
	{
		ini.loadFromFile(APPDATA + "/leveld-editor.ini");
	}
	catch (...)
	{
		ini["Window"]["title"] = "LevelD file editor";
		ini["Window"]["width"] = 1280;
		ini["Window"]["height"] = 720;
	}

	dgm::WindowSettings windowSettings = {
		.resolution = sf::Vector2u(ini["Window"]["width"].asInt(), ini["Window"]["height"].asInt()),
		.title = ini["Window"]["title"].asString(),
		.useFullscreen = false
	};

	dgm::Window window(windowSettings);
	dgm::App app(window);

	auto shortcutEngine = std::make_unique<ShortcutEngine>();
	app.pushState<AppStateEditor>(
		ini,
		rootDir,
		std::move(fileApi),
		std::move(shortcutEngine));
	app.run();

	windowSettings = window.close();

	ini["Window"]["width"] = int(windowSettings.resolution.x);
	ini["Window"]["height"] = int(windowSettings.resolution.y);
	ini["Window"]["title"] = windowSettings.title;

	ini.saveToFile(APPDATA + "/leveld-editor.ini");

	return 0;
}