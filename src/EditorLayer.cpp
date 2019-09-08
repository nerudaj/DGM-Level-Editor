#include "EditorLayer.hpp"

void EditorLayerTile::init(unsigned width, unsigned height, sf::Vector2i& tileSize, EditorBrush& brush) {
	EditorBrushTile& tileBrush = dynamic_cast<EditorBrushTile&>(brush);
	tileData = std::vector<int>(size_t(width) * size_t(height), 0);

	tileset.setTexture(tileBrush.getTexture());
	tileset.build(tileBrush.getClip(), sf::Vector2u(tileSize), tileData, { width, height });

	meshMap = tileBrush.getMeshMap();
	mesh.setDataSize({width, height});
	mesh.setVoxelSize(sf::Vector2u(tileSize));
}

void EditorLayerTile::saveToLevelD(LevelD& lvd) const {
	lvd.mesh.width = mesh.getDataSize().x;
	lvd.mesh.height = mesh.getDataSize().y;

	lvd.mesh.tiles = std::vector<uint16_t>(tileData.begin(), tileData.end());
	
	unsigned size = lvd.mesh.width * lvd.mesh.height;
	lvd.mesh.blocks.resize(size, 0);
	for (unsigned i = 0; i < size; i++) {
		lvd.mesh.blocks[i] = mesh[i];
	}
}

void EditorLayerTile::loadFromLevelD(const LevelD& lvd) {
	unsigned size = lvd.mesh.width * lvd.mesh.height;
	mesh.setDataSize({ lvd.mesh.width, lvd.mesh.height });
	for (unsigned i = 0; i < size; i++) {
		mesh[i] = lvd.mesh.blocks[i];
	}

	// TODO: voxel size

	tileData = std::vector<int>(lvd.mesh.tiles.begin(), lvd.mesh.tiles.end());
	tileset.build(tileset.getClip(), mesh.getVoxelSize(), tileData, mesh.getDataSize());
}

void EditorLayerItem::saveToLevelD(LevelD& lvd) const {
	itemset.saveToLeveld(lvd);
}

void EditorLayerItem::loadFromLevelD(const LevelD& lvd) {
	itemset.loadFromLeveld(lvd);
}
