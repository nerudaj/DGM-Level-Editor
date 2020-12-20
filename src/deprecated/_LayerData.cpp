#include "Layer.hpp"

unsigned LayerDataTile::getNormalizedIndex(unsigned x, unsigned y) const {
	x /= tileSize.x;
	y /= tileSize.y;
	return y * dataSize.x + x;
}

void LayerDataTile::draw() {
	sf::Sprite sprite;
	sprite.setTexture(textureHandler.getSfmlTexture());

	for (unsigned y = 0, i = 0; y < dataSize.y; y++) {
		for (unsigned x = 0; x < dataSize.x; x++, i++) {
			sprite.setTextureRect(textureHandler.getTextureRect(data[i]));
			sprite.setPosition(tileSize.x * (x + originOffsetX), tileSize.y * (y + originOffsetY));
			canvas->draw(sprite);
		}
	}

	background.draw(canvas);
}

void LayerDataTile::set(int x, int y, const LayerProperty& prop) {
	const unsigned normalizedIndex = getNormalizedIndex(x, y);
	if (normalizedIndex >= data.size()) return;
	const LayerPropertyTile& tileProp = dynamic_cast<const LayerPropertyTile&>(prop);	
	data[normalizedIndex] = tileProp;
}

const LayerProperty& LayerDataTile::get(unsigned x, unsigned y) const {
	return data[getNormalizedIndex(x, y)];
}

void LayerDataTile::resize(unsigned width, unsigned height) {
	if (width * height > 1000000000) {
		throw std::runtime_error("Requesting too much memory");
	}

	auto backup = data;
	data.resize(width * height, LayerPropertyTile());
	centerBackupInResizedData(backup, width, height);
	dataSize = sf::Vector2u(width, height);

	background.build(dataSize, tileSize);
	background.setPosition(originOffsetX * tileSize.x, originOffsetY * tileSize.y);
}

void LayerDataTile::init() {
	tileSize = textureHandler.getTextureRectSize();
	resize(40, 20);
}

void LayerDataTile::centerBackupInResizedData(std::vector<LayerPropertyTile>& backup, unsigned int width, unsigned int height) {
	if (backup.size() == 0) return;

	unsigned offsetX = (width - dataSize.x) / 2u;
	unsigned offsetY = (height - dataSize.y) / 2u;

	for (unsigned y = 0, i = 0; y < dataSize.y; y++) {
		for (unsigned x = 0; x < dataSize.x; x++, i++) {
			unsigned j = (y + offsetY) * width + (x + offsetX);
			data[j] = backup[i];
		}
	}
}
