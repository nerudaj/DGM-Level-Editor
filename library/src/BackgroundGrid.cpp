#include "BackgroundGrid.hpp"

void BackgroundGrid::build(const sf::Vector2u& gridSize, const sf::Vector2u& tileSize) {
	sf::Image image;
	image.create(tileSize.x * 2, tileSize.y);

	for (unsigned y = 0; y < tileSize.y; y++) {
		for (unsigned x = 0; x < tileSize.x; x++) {
			image.setPixel(x, y, sf::Color::Transparent);
			image.setPixel(x + tileSize.x, y, sf::Color(255, 255, 255, 16));
		}
	}

	texture.loadFromImage(image);

	std::vector<int> bgrData(size_t(gridSize.x) * gridSize.y, 0);
	for (unsigned y = 0; y < gridSize.y; y++) {
		for (unsigned x = y % 2; x < gridSize.x; x += 2) {
			bgrData[size_t(y) * gridSize.x + x] = 1;
		}
	}

	tilemap.init(texture, dgm::Clip(tileSize, sf::IntRect(0, 0, tileSize.x * 2, tileSize.y)));
	tilemap.build(tileSize, bgrData, gridSize);
}
