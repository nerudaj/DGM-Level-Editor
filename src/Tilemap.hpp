#pragma once

#include <DGM/DGM.hpp>

class Tilemap : public dgm::TileMap {
public:
    dgm::Mesh mesh;

    unsigned getTile(unsigned tileX, unsigned tileY);

    bool getTileBlock(unsigned tileX, unsigned tileY) {
        return mesh.at(tileX, tileY);
    }

    void setTile(unsigned x, unsigned y, unsigned tileValue, bool blocking) {
        changeTile(x, y, tileValue);
        mesh.at(x, y) = blocking;
    }

    void drawArea(const sf::Vector2i& start, const sf::Vector2i& end, bool fill, unsigned tileValue, bool blocking);
};