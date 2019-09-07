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

	void setZoomLevel(float zoomLevel) {
		Itemset::zoomLevel = zoomLevel;

		unsigned i = 0;
		for (unsigned y = 0; y < map.size() / width; y++) {
			for (unsigned x = 0; x < width; x++, i++) {
				if (map[i] == -1) continue;

				items[map[i]].sprite.setPosition(tileSize.x * x * zoomLevel, tileSize.y * y * zoomLevel);
				items[map[i]].sprite.setScale(zoomLevel, zoomLevel);
			}
		}
	}

	void addItem(unsigned x, unsigned y, unsigned value) {
		if (map[y * width + x] == -1) {
			items.push_back({ sf::Sprite(), int(value) });
			items.back().sprite.setTexture(brush->getTextureForItem(value));
			items.back().sprite.setTextureRect(brush->getTextureRectForItem(value));
			items.back().sprite.setPosition(tileSize.x * x * zoomLevel, tileSize.y * y * zoomLevel);
			items.back().sprite.setScale(zoomLevel, zoomLevel);
			map[y * width + x] = items.size() - 1;
		}
		else {
			unsigned i = map[y * width + x];
			items[i].sprite.setTexture(brush->getTextureForItem(value));
			items[i].sprite.setTextureRect(brush->getTextureRectForItem(value));
			items[i].id = value;
		}
	}

	void initialize(unsigned width, unsigned height, sf::Vector2f size, EditorBrushItem& brush) {
		map.resize(width * height, -1);
		Itemset::width = width;
		tileSize = size;
		Itemset::brush = &brush;
	}

	void saveToLeveld(LevelD& lvd) const {
		lvd.items.clear();

		unsigned i = 0;
		for (unsigned y = 0; y < map.size() / width; y++) {
			for (unsigned x = 0; x < width; x++, i++) {
				if (map[i] == -1) continue;

				lvd.items.push_back({
					uint32_t(x * tileSize.x), 
					uint32_t(y * tileSize.y), 
					uint16_t(items[map[i]].id) 
				});
			}
		}
	}

	void loadFromLeveld(const LevelD& lvd) {
		items.clear();
		std::fill(map.begin(), map.end(), -1);

		for (auto& item : lvd.items) {
			addItem(item.x / tileSize.x, item.y / tileSize.y, item.flags);
		}
	}

	Itemset() {
		zoomLevel = 1.f;
	}
};