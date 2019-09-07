#pragma once

#include <DGM/dgm.hpp>
#include <TGUI/TGUI.hpp>
#include "EditorBrush.hpp"
#include "Itemset.hpp"

class EditorLayer {
public:
	virtual void draw(tgui::Canvas::Ptr canvas) = 0;
	virtual void changeTile(unsigned x, unsigned y, unsigned value) = 0;
	virtual void setZoomLevel(float zoom) = 0;
	virtual void init(unsigned width, unsigned height, sf::Vector2i& tileSize, EditorBrush& brush) = 0;
	virtual void saveToLevelD(LevelD& lvd) const = 0;
	virtual void loadFromLevelD(const LevelD& lvd) = 0;
};

class EditorLayerTile : public EditorLayer {
private:
	dgm::Tileset tileset;
	dgm::Mesh mesh;
	std::vector<bool> meshMap;
	std::vector<int> tileData;

public:
	virtual void draw(tgui::Canvas::Ptr canvas) override {
		canvas->draw(tileset);
	}

	virtual void changeTile(unsigned x, unsigned y, unsigned value) override {
		tileset.changeTile(x, y, value);
		tileData[size_t(y) * mesh.getDataSize().x + x] = value;
		mesh[size_t(y) * mesh.getDataSize().x + x] = meshMap[value];
	}

	virtual void setZoomLevel(float zoom) override {
		tileset.setScale(zoom, zoom);
	}

	virtual void init(unsigned width, unsigned height, sf::Vector2i& tileSize, EditorBrush& brush);

	virtual void saveToLevelD(LevelD& lvd) const override;

	virtual void loadFromLevelD(const LevelD& lvd) override;
};

class EditorLayerItem : public EditorLayer {
private:
	Itemset itemset;

public:
	// Inherited via EditorLayer
	virtual void draw(tgui::Canvas::Ptr canvas) override {
		itemset.draw(canvas);
	}

	virtual void changeTile(unsigned x, unsigned y, unsigned value) override {
		itemset.addItem(x, y, value);
	}

	virtual void setZoomLevel(float zoom) override {
		itemset.setZoomLevel(zoom);
	}

	virtual void init(unsigned width, unsigned height, sf::Vector2i& tileSize, EditorBrush& brush) override {
		itemset.initialize(width, height, sf::Vector2f(tileSize), dynamic_cast<EditorBrushItem&>(brush));
	}

	virtual void saveToLevelD(LevelD& lvd) const override;

	virtual void loadFromLevelD(const LevelD& lvd) override;
};