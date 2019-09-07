#include "Editor.hpp"

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
		activeTool = tileTool;
		activeLayer = tileLayer;
	}
	else {
		activeHistory = itemHistory;
		activeTool = itemTool;
		activeLayer = itemLayer;
	}
}

void Editor::init(unsigned width, unsigned height, const dgm::ResourceManager& resmgr) {
	tileSize = { 16, 16 }; // TODO: setup of this

	tileHistory.addItem(0);
	itemHistory.addItem(0);

	tileTool.init("TODO", resmgr);
	itemTool.init("TODO", resmgr);

	tileLayer.init(width, height, tileSize, tileTool);
	itemLayer.init(width, height, tileSize, itemTool);
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