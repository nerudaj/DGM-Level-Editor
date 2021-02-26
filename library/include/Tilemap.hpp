#pragma once

#include <DGM/DGM.hpp>

class Overlay {
public:
    dgm::TileMap map;
    sf::Texture texture;
    dgm::Clip clip;

    void init() {
        sf::Image img;
        img.create(2, 1);
        img.setPixel(0, 0, sf::Color::Transparent);
        img.setPixel(1, 0, sf::Color(0, 0, 255, 96));
        texture.loadFromImage(img);

        clip.init({ 1, 1 }, { 0, 0, 2, 1 });

        map.init(texture, clip);
    }
};

class Tilemap : public dgm::TileMap {
public:
    dgm::Mesh mesh;
    Overlay overlay;

    unsigned getTile(unsigned tileX, unsigned tileY);

    bool getTileBlock(unsigned tileX, unsigned tileY) {
        return mesh.at(tileX, tileY);
    }

    void setTile(unsigned x, unsigned y, unsigned tileValue, bool blocking) {
        changeTile(x, y, tileValue);
        mesh.at(x, y) = blocking;
        overlay.map.changeTile(x, y, blocking);
    }

    void drawArea(const sf::Vector2i& start, const sf::Vector2i& end, bool fill, unsigned tileValue, bool blocking);
};