#include <DGM/dgm.hpp>
#include <Config.hpp>

#include "include/AppStateEditor.hpp"
#include "include/Utilities/FileApi.hpp"
#include "include/Shortcuts/ShortcutEngine.hpp"
#include "include/Utilities/GC.hpp"
#include "include/Dialogs/YesNoCancelDialog.hpp"
#include "include/Dialogs/ErrorInfoDialog.hpp"
#include "include/ProgramOptions.hpp"
#include "include/Gui.hpp"

#include <string>
#include <functional>
#include <format>

std::filesystem::path GetBinaryDirectory()
{
	char buffer[MAX_PATH];
	GetModuleFileName(NULL, buffer, MAX_PATH);
	return std::filesystem::path(buffer).parent_path();
}

std::string ComputeShortHash(const std::string& str)
{
	return std::format("{:x}", std::hash<std::string>{}(str));
}

int main(int argc, char* argv[])
{
	std::string rootDir = "..";
	if (argc == 2)
	{
		rootDir = argv[1];
	}

	auto binaryDirectory = GetBinaryDirectory();
	ProgramOptions programOptions{
		.binaryDir = binaryDirectory,
		.rootDir = binaryDirectory / rootDir,
		.binaryDirHash = ComputeShortHash(binaryDirectory.string())
	};

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

	// Workaround to get resizable window until dgm-lib allows to do so directly
	window.getWindowContext().close();
	window.getWindowContext().create(
		{ windowSettings.resolution.x, windowSettings.resolution.y },
		windowSettings.title,
		sf::Style::Default);

	dgm::App app(window);

	GC<Gui> gui;

	app.pushState<AppStateEditor>(
		gui,
		ini,
		programOptions,
		fileApi,
		GC<ShortcutEngine>([&gui] { return gui->isAnyModalOpened(); }),
		GC<YesNoCancelDialog>(gui),
		GC<ErrorInfoDialog>(gui));
	app.run();

	windowSettings = window.close();

	ini["Window"]["width"] = int(windowSettings.resolution.x);
	ini["Window"]["height"] = int(windowSettings.resolution.y);
	ini["Window"]["title"] = windowSettings.title;

	ini.saveToFile(APPDATA + "/leveld-editor.ini");

	return 0;
}