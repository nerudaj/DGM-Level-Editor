#pragma once

#include <DGM/DGM.hpp>

class Overlay
{
protected:
	sf::Texture texture;

public:
	dgm::TileMap map;

	Overlay()
	{
		sf::Image img;
		img.create(2, 1);
		img.setPixel(0, 0, sf::Color::Transparent);
		img.setPixel(1, 0, sf::Color(0, 0, 255, 96));

		if (!texture.loadFromImage(img))
		{
			throw std::runtime_error("Cannot build overlay texture");
		}

		map = dgm::TileMap(
			texture,
			dgm::Clip({ 1, 1 }, { 0, 0, 2, 1 }));
		// TODO: Test scenario
		// 1) open level 1
		// 2) turn on overlay
		// 3) open level 2
		// 4) check overlay still working
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

	/*void buildEmpty(unsigned width, unsigned height, const sf::Vector2u& frameSize)
	{
		build(frameSize, std::vector<int>(width * height, 0), { width, height });
		overlay.map.build(frameSize, std::vector<int>(width * height, 0), { width, height });
		mesh.setDataSize(width, height);
	}*/

	void buildAll(
		const std::vector<int>& tiles,
		const std::vector<int>& blocks,
		const sf::Vector2u& dataSize)
	{
		build(clip.getFrameSize(), tiles, dataSize);
		mesh = dgm::Mesh(blocks, dataSize, clip.getFrameSize());
		overlay.map.build(clip.getFrameSize(), blocks, dataSize);
	}

	bool empty()
	{
		return !(mesh.getDataSize().x * mesh.getDataSize().y);
	}

	Tilemap() = default;
	Tilemap(sf::Texture& texture, dgm::Clip clip)
		: dgm::TileMap(texture, std::move(clip))
	{}
};