#include <DGM/dgm.hpp>

#include "include/AppStateEditor.hpp"
#include "include/FileApi.hpp"

int main(int argc, char *argv[]) {
	std::string rootDir = "..";
	if (argc == 2) {
		rootDir = argv[1];
	}

	const auto APPDATA = FileApi::resolveAppdata();

	cfg::Ini ini;
	try {
		ini.loadFromFile(APPDATA + "/leveld-editor.ini");
	} catch (...) {
		ini["Window"]["title"] = "LevelD file editor";
		ini["Window"]["width"] = 1280;
		ini["Window"]["height"] = 720;
	}

	dgm::Window window(ini);
	dgm::App app(window);
	
	app.pushState<AppStateEditor>(ini, rootDir);
	app.run();

	window.close(ini);

	ini.saveToFile(APPDATA + "/leveld-editor.ini");

	return 0;
}