#include <DGM/dgm.hpp>

#include "AppStateEditor.hpp"

int main(int argc, char *argv[]) {
	std::string rootDir = "..";
	if (argc == 2) {
		rootDir = argv[1];
	}

	cfg::Ini ini;
	try {
		ini.loadFromFile(rootDir + "/app.ini");
	} catch (...) {
		ini["Window"]["title"] = "LevelD file editor";
		ini["Window"]["width"] = 1280;
		ini["Window"]["height"] = 720;
	}

	dgm::App app;
	app.window.open(ini);
	
	app.pushState(new AppStateEditor(rootDir));
	app.run();

	app.window.close(ini);

	ini.saveToFile(rootDir + "/app.ini");

	return 0;
}