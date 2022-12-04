#pragma once

#include <DGM/DGM.hpp>

class Overlay
{
public:
	dgm::TileMap map;
	sf::Texture texture;
	dgm::Clip clip;

	void init()
	{
		// NOTE: this could be loaded via resmgr or not?
		sf::Image img;
		img.create(2, 1);
		img.setPixel(0, 0, sf::Color::Transparent);
		img.setPixel(1, 0, sf::Color(0, 0, 255, 96));
		img.saveToFile("pokus.png");

		if (!texture.loadFromImage(img))
		{
			throw std::runtime_error("Cannot build overlay texture");
		}

		clip.init({ 1, 1 }, { 0, 0, 2, 1 });

		map = dgm::TileMap(texture, std::move(clip));
		// FIXME: this deletes all data set in ToolMesh::resize
	}
};

class Tilemap : public dgm::TileMap
{
public:
	dgm::Mesh mesh;
	Overlay overlay;

	unsigned getTile(unsigned tileX, unsigned tileY);

	bool getTileBlock(unsigned tileX, unsigned tileY)
	{
		return mesh.at(tileX, tileY);
	}

	void setTile(unsigned x, unsigned y, unsigned tileValue, bool blocking)
	{
		changeTile(x, y, tileValue);
		mesh.at(x, y) = blocking;
		overlay.map.changeTile(x, y, blocking);
	}

	void drawArea(const sf::Vector2i& start, const sf::Vector2i& end, bool fill, unsigned tileValue, bool blocking);

	void buildAll(unsigned width, unsigned height, const sf::Vector2u& frameSize)
	{
		build(frameSize, std::vector<int>(width * height, 0), { width, height });
		overlay.map.build(frameSize, std::vector<int>(width * height, 0), { width, height });
		mesh.setDataSize(width, height);
	}

	bool empty()
	{
		return !(mesh.getDataSize().x * mesh.getDataSize().y);
	}

	Tilemap() = default;
	Tilemap(sf::Texture& texture, dgm::Clip clip)
		: dgm::TileMap(texture, std::move(clip))
	{
		mesh.setDataSize(0, 0);
	}
};