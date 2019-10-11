#pragma once

#include "EditorMode.hpp"
#include "BrushHistory.hpp"
#include "ToolRenderer.hpp"
#include "EditorLayer.hpp"

/**
 *  @brief Data context of Level editor
 */
class Editor {
private:
	// Resources
	dgm::ResourceManager resmgr;
	sf::Texture backgroundTexture;
	dgm::Tileset background;

	// Histories
	BrushHistory tileHistory;
	BrushHistory itemHistory;
	std::reference_wrapper<BrushHistory> activeHistory;

	// Tools
	ToolRendererTile tileBrush;
	ToolRendererItem itemBrush;
	std::reference_wrapper<ToolRenderer> activeBrush;

	// Layers
	EditorLayerTile tileLayer;
	EditorLayerItem itemLayer;
	std::reference_wrapper<EditorLayer> activeLayer;

	// Misc
	sf::Vector2i tileSize;

	void buildBackground(const sf::Vector2i& tileSize, unsigned width, unsigned height);

public:
	void draw(tgui::Canvas::Ptr canvas);

	void setZoomLevel(float zoom);

	const sf::Vector2i& getTileSize() const {
		return tileSize;
	}

	BrushHistory& getActiveHistory() {
		return activeHistory;
	}

	const BrushHistory& getActiveHistory() const {
		return activeHistory.get();
	}

	ToolRenderer& getActiveBrush() {
		return activeBrush.get();
	}

	const ToolRenderer& getActiveBrush() const {
		return activeBrush;
	}

	EditorLayer& getActiveLayer() {
		return activeLayer.get();
	}

	const EditorLayer& getActiveLayer() const {
		return activeLayer.get();
	}

	void setMode(EditorMode mode);

	void init(unsigned width, unsigned height, const std::string &configPath);

	/**
	 *  \warn Broken if init wasnt called first!
	 *  \todo FIX
	 */
	void saveToFile(const std::string& filename);

	/**
	 *  \warn Broken if init wasnt called first!
	 *  \todo FIX
	 */
	void loadFromFile(const std::string& filename);

	Editor() : activeHistory(tileHistory), activeBrush(tileBrush), activeLayer(tileLayer) {
		tileHistory.clear();
		itemHistory.clear();
	}
};