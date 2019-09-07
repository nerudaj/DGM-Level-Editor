#pragma once

#include "EditorMode.hpp"
#include "EditorHistory.hpp"
#include "EditorBrush.hpp"
#include "EditorLayer.hpp"

/**
 *  @brief Data context of Level editor
 */
class Editor {
private:
	// Histories
	EditorHistory tileHistory;
	EditorHistory itemHistory;
	std::reference_wrapper<EditorHistory> activeHistory;

	// Tools
	EditorBrushTile tileTool;
	EditorBrushItem itemTool;
	std::reference_wrapper<EditorBrush> activeTool;

	// Layers
	EditorLayerTile tileLayer;
	EditorLayerItem itemLayer;
	std::reference_wrapper<EditorLayer> activeLayer;

	// Misc
	sf::Vector2i tileSize;

public:
	void draw(tgui::Canvas::Ptr canvas);

	void setZoomLevel(float zoom);

	const sf::Vector2i& getTileSize() const {
		return tileSize;
	}

	EditorHistory& getActiveHistory() {
		return activeHistory;
	}

	const EditorHistory& getActiveHistory() const {
		return activeHistory.get();
	}

	EditorBrush& getActiveTool() {
		return activeTool.get();
	}

	const EditorBrush& getActiveTool() const {
		return activeTool;
	}

	EditorLayer& getActiveLayer() {
		return activeLayer.get();
	}

	const EditorLayer& getActiveLayer() const {
		return activeLayer.get();
	}

	void setMode(EditorMode mode);

	void init(unsigned width, unsigned height, const dgm::ResourceManager& resmgr);

	void saveToFile(const std::string& filename);

	void loadFromFile(const std::string& filename);

	Editor() : activeHistory(tileHistory), activeTool(tileTool), activeLayer(tileLayer) {}
};