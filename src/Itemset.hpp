#pragma once

#include <algorithm>
#include <DGM/dgm.hpp>
#include <TGUI/TGUI.hpp>
#include "EditorBrush.hpp"

class Itemset {
private:
	struct Item {
		sf::Sprite sprite;
		int id;
	};

	std::vector<Item> items;
	std::vector<int> map;
	unsigned width;
	sf::Vector2f tileSize;
	float zoomLevel;
	EditorBrushItem *brush;

public:
	void draw(tgui::Canvas::Ptr canvas) {
		for (auto& item : items) {
			canvas->draw(item.sprite);
		}
	}

	void setZoomLevel(float zoomLevel);

	void addItem(unsigned x, unsigned y, unsigned value);

	void initialize(unsigned width, unsigned height, sf::Vector2f size, EditorBrushItem& brush) {
		map.resize(width * height, -1);
		Itemset::width = width;
		tileSize = size;
		Itemset::brush = &brush;
	}

	void saveToLeveld(LevelD& lvd) const;

	void loadFromLeveld(const LevelD& lvd);

	Itemset() {
		zoomLevel = 1.f;
	}
};