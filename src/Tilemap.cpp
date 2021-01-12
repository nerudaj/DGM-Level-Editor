#include "Tilemap.hpp"
#include "LogConsole.hpp"

unsigned Tilemap::getTile(unsigned tileX, unsigned tileY) {
	unsigned tileI = tileY * dataSize.x + tileX;
	sf::Vertex *quad = &vertices[tileI * size_t(4)];

	for (unsigned i = 0; i < clip.getFrameCount(); i++) {
		if (quad[0].texCoords.x == clip.getFrame(i).left && quad[0].texCoords.y == clip.getFrame(i).top) return i;
	}

	Log::write("Value of tile on coords[" + std::to_string(tileX) + ", " + std::to_string(tileY) + "] not found");
	return 0;
}

void Tilemap::drawArea(const sf::Vector2i& start, const sf::Vector2i& end, bool fill, unsigned tileValue, bool blocking) {
	unsigned startX = start.x / mesh.getVoxelSize().x;
	unsigned startY = start.y / mesh.getVoxelSize().y;
	unsigned endX = end.x / mesh.getVoxelSize().x;
	unsigned endY = end.y / mesh.getVoxelSize().y;

	Log::write("drawArea [" + std::to_string(startX) + ", " + std::to_string(startY) + "] -> [" +
		std::to_string(endX) + ", " + std::to_string(endY) + "]");

	for (unsigned y = startY; y <= endY; y++) {
		for (unsigned x = startX; x <= endX; x++) {
			if (!fill && !(x == startX || x == endX || y == startY || y == endY)) continue;

			setTile(x, y, tileValue, blocking);
		}
	}
}