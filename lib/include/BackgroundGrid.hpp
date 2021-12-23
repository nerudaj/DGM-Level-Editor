#pragma once

#include <DGM/dgm.hpp>
#include <TGUI/TGUI.hpp>

class BackgroundGrid {
private:
	dgm::TileMap tilemap;
	sf::Texture texture;

public:
	void draw(tgui::Canvas::Ptr canvas) {
		canvas->draw(tilemap);
	}
	
	void build(const sf::Vector2u& gridSize, const sf::Vector2u& tileSize);

	void setPosition(float x, float y) {
		tilemap.setPosition(x, y);
	}
};