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

void Editor::buildBackground(const sf::Vector2i& tileSize, unsigned width, unsigned height) {
	sf::Image image;
	image.create(tileSize.x * 2, tileSize.y);

	for (int y = 0; y < tileSize.y; y++) {
		for (int x = 0; x < tileSize.x; x++) {
			image.setPixel(x, y, sf::Color::Transparent);
			image.setPixel(x + tileSize.x, y, sf::Color(255, 255, 255, 16));
		}
	}

	backgroundTexture.loadFromImage(image);
	
	std::vector<int> bgrData(size_t(width) * height, 0);
	for (unsigned y = 0; y < height; y++) {
		for (unsigned x = y % 2; x < width; x += 2) {
			bgrData[y * width + x] = 1;
		}
	}

	background.setTexture(backgroundTexture);
	background.build(dgm::Clip(tileSize, sf::IntRect(0, 0, tileSize.x * 2, tileSize.y)), sf::Vector2u(tileSize), bgrData, { width, height });
}

void Editor::draw(tgui::Canvas::Ptr canvas) {
	tileLayer.draw(canvas);
	canvas->draw(background);
	itemLayer.draw(canvas);
}

void Editor::setZoomLevel(float zoom) {
	background.setScale(zoom, zoom);
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
	resmgr.loadResourceDir<sf::Texture>(basePath + "/textures");
	resmgr.loadResourceDir<std::shared_ptr<dgm::AnimationStates>>(basePath + "/configs");

	tileSize.x = config["editor"]["tileSize"][0].get<int>();
	tileSize.y = config["editor"]["tileSize"][1].get<int>();

	buildBackground(tileSize, width, height);

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

	buildBackground(tileSize, lvd.mesh.width, lvd.mesh.height);
}

std::string std::to_string(EditorMode mode) {
	if (mode == EditorMode::Tiles) return "Tiles";
	return "Items";
}