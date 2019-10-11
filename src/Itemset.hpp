#pragma once

#include <algorithm>
#include <DGM/dgm.hpp>
#include <TGUI/TGUI.hpp>
#include "ToolRenderer.hpp"

class Itemset {
private:
	struct Item {
		sf::Sprite sprite;
		int id;
		uint16_t flags;
	};

	std::vector<Item> items;
	std::vector<int> map;
	unsigned width;
	sf::Vector2f tileSize;
	float zoomLevel;
	ToolRendererItem *brush;

public:
	void draw(tgui::Canvas::Ptr canvas) {
		for (auto& item : items) {
			canvas->draw(item.sprite);
		}
	}

	void setZoomLevel(float zoomLevel);

	void addItem(unsigned x, unsigned y, unsigned value);

	void removeItem(unsigned x, unsigned y);

	void setProperties(unsigned x, unsigned y, uint16_t flags);

	sf::Vector2u getSize() const {
		return { width, static_cast<unsigned>(map.size() / width) };
	}

	uint16_t getItemProperties(unsigned x, unsigned y) {
		if (map[y * width + x] == -1) return 0;
		return items[map[y * width + x]].flags;
	}

	int getItemValue(unsigned x, unsigned y) {
		if (map[y * width + x] == -1) return -1;
		return items[map[y * width + x]].id;
	}

	void initialize(unsigned width, unsigned height, sf::Vector2f size, ToolRendererItem& brush) {
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