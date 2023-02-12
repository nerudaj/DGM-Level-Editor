#include <DGM/dgm.hpp>
#include <Config.hpp>

#include "include/AppStateEditor.hpp"
#include "include/Utilities/FileApi.hpp"
#include "include/Shortcuts/ShortcutEngine.hpp"
#include "include/Utilities/GC.hpp"
#include "include/Dialogs/YesNoCancelDialog.hpp"
#include "include/Dialogs/ErrorInfoDialog.hpp"

int main(int argc, char* argv[])
{
	std::string rootDir = "..";
	if (argc == 2)
	{
		rootDir = argv[1];
	}

	auto fileApi = GC<FileApi>();
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

	tgui::Gui gui;
	tgui::Theme theme;

	app.pushState<AppStateEditor>(
		gui,
		theme,
		ini,
		rootDir,
		fileApi,
		GC<ShortcutEngine>(),
		GC<YesNoCancelDialog>(gui, theme),
		GC<ErrorInfoDialog>(gui, theme));
	app.run();

	windowSettings = window.close();

	ini["Window"]["width"] = int(windowSettings.resolution.x);
	ini["Window"]["height"] = int(windowSettings.resolution.y);
	ini["Window"]["title"] = windowSettings.title;

	ini.saveToFile(APPDATA + "/leveld-editor.ini");

	return 0;
}