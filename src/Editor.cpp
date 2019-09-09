#include "Editor.hpp"
#include <json.hpp>
#include <fstream>

nlohmann::json loadJsonFromFile(const std::string& filename) {
	std::ifstream load(filename);
	nlohmann::json file;
	load >> file;
	load.close();
	load.clear();

	return file;
}

void Editor::draw(tgui::Canvas::Ptr canvas) {
	tileLayer.draw(canvas);
	itemLayer.draw(canvas);
}

void Editor::setZoomLevel(float zoom) {
	tileLayer.setZoomLevel(zoom);
	itemLayer.setZoomLevel(zoom);
}

void Editor::setMode(EditorMode mode) {
	if (mode == EditorMode::Tiles) {
		activeHistory = tileHistory;
		activeBrush = tileBrush;
		activeLayer = tileLayer;
	}
	else {
		activeHistory = itemHistory;
		activeBrush = itemBrush;
		activeLayer = itemLayer;
	}
}

void Editor::init(unsigned width, unsigned height, const std::string &configPath) {
	auto config = loadJsonFromFile(configPath);

	std::string basePath = config["editor"]["pathToGameResources"].get<std::string>() + "/graphics";
	// TODO: remember path to binary
	resmgr.loadResourceDir<sf::Texture>(basePath + "/textures");
	resmgr.loadResourceDir<std::shared_ptr<dgm::AnimationStates>>(basePath + "/configs");

	tileSize.x = config["editor"]["tileSize"][0].get<int>();
	tileSize.y = config["editor"]["tileSize"][1].get<int>();

	tileHistory.clear();
	itemHistory.clear();

	tileHistory.addItem(0);
	itemHistory.addItem(0);

	tileBrush.init(config, resmgr);
	itemBrush.init(config, resmgr);

	tileLayer.init(width, height, tileSize, tileBrush);
	itemLayer.init(width, height, tileSize, itemBrush);
}

void Editor::saveToFile(const std::string& filename) {
	LevelD lvd;

	tileLayer.saveToLevelD(lvd);
	itemLayer.saveToLevelD(lvd);

	lvd.saveToFile(filename);
}

void Editor::loadFromFile(const std::string& filename) {
	LevelD lvd;
	lvd.loadFromFile(filename);

	tileLayer.loadFromLevelD(lvd);
	itemLayer.loadFromLevelD(lvd);
}

std::string std::to_string(EditorMode mode) {
	if (mode == EditorMode::Tiles) return "Tiles";
	return "Items";
}