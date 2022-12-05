#include "include/LevelMesh/DrawableLeveldMesh.hpp"
#include "include/LogConsole.hpp"

void DrawableLeveldMesh::setTileArea(
	const sf::Vector2i& start,
	const sf::Vector2i& end,
	bool fill,
	unsigned tileValue,
	bool blocking)
{
	const auto _mesh = mesh.getOriginalMesh();

	const unsigned aX = (start.x < 0 ? 0 : start.x) / _mesh.getVoxelSize().x;
	const unsigned aY = (start.y < 0 ? 0 : start.y) / _mesh.getVoxelSize().y;
	const unsigned bX = (end.x < 0 ? 0 : end.x) / _mesh.getVoxelSize().x;
	const unsigned bY = (end.y < 0 ? 0 : end.y) / _mesh.getVoxelSize().y;

	const unsigned startX = std::min(aX, bX);
	const unsigned startY = std::min(aY, bY);
	const unsigned endX = std::max(aX, bX) < _mesh.getDataSize().x ? std::max(aX, bX) : _mesh.getDataSize().x - 1;
	const unsigned endY = std::max(aY, bY) < _mesh.getDataSize().y ? std::max(aY, bY) : _mesh.getDataSize().y - 1;

	Log::write("drawArea [" + std::to_string(startX) + ", " + std::to_string(startY) + "] -> [" +
		std::to_string(endX) + ", " + std::to_string(endY) + "]");

	for (unsigned y = startY; y <= endY; y++)
	{
		for (unsigned x = startX; x <= endX; x++)
		{
			if (!fill && !(x == startX || x == endX || y == startY || y == endY)) continue;

			setTileValue({ x, y }, tileValue);
			setTileSolid({ x, y }, blocking);
		}
	}
}

void DrawableLeveldMesh::drawTo(tgui::Canvas::Ptr canvas, bool renderCollisionMesh)
{
	canvas->draw(map);
	if (renderCollisionMesh)
		mesh.drawTo(canvas);
	grid.drawTo(canvas);
}

void DrawableLeveldMesh::build(
	const std::vector<int>& tiles,
	const std::vector<int>& blocks,
	const sf::Vector2u& dataSize)
{
	const auto tileSize = map.getClip().getFrameSize();
	map.build(tileSize, tiles, dataSize);
	mesh.build(blocks, dataSize, tileSize);
	grid.build(dataSize, tileSize);
}
