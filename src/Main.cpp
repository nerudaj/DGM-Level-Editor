#include <DGM/dgm.hpp>

#include "AppStateMainMenu.hpp"
#include "AppStateMenuOptions.hpp"
#include "AppStateIngame.hpp"

int main(int argc, char *argv[]) {
	std::string rootDir = "..";
	if (argc == 2) {
		rootDir = argv[1];
	}

	cfg::Ini ini;
	try {
		ini.loadFromFile(rootDir + "/app.ini");
	} catch (...) {}

	dgm::App app;
	app.window.open(ini);
	
	app.pushState(new AppStateMainMenu(rootDir));
	app.run();

	app.window.close(ini);

	ini.saveToFile(rootDir + "/app.ini");

	return 0;
}