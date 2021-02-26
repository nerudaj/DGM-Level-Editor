#include <DGM/dgm.hpp>

#include <AppStateEditor.hpp>
#include <FileApi.hpp>

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

	dgm::App app;
	app.window.open(ini);
	
	app.pushState(new AppStateEditor(ini, rootDir));
	app.run();

	app.window.close(ini);

	ini.saveToFile(APPDATA + "/leveld-editor.ini");

	return 0;
}