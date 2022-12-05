#pragma once

#include <DGM/classes/Objects.hpp>
#include <DGM/classes/TileMap.hpp>
#include <TGUI/Widgets/Canvas.hpp>

class DrawableDgmMesh final
{
protected:
	sf::Texture texture;
	dgm::TileMap map;
	dgm::Mesh mesh;

public:
	DrawableDgmMesh();
	DrawableDgmMesh(DrawableDgmMesh&&) = delete;
	DrawableDgmMesh(const DrawableDgmMesh&) = delete;

public:
	void build(
		const std::vector<int>& data,
		const sf::Vector2u& dataSize,
		const sf::Vector2u& voxelSize);

	[[nodiscard]]
	const dgm::Mesh& getOriginalMesh() const
	{
		return mesh;
	}

	[[nodiscard]]
	bool isTileSolid(unsigned x, unsigned y) const
	{
		return mesh.at(x, y);
	}

	void setTileSolid(unsigned x, unsigned y, bool isSolid)
	{
		mesh.at(x, y) = isSolid;
		map.changeTile(x, y, isSolid);
	}

	void drawTo(tgui::Canvas::Ptr canvas)
	{
		canvas->draw(map);
	}
};