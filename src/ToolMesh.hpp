#pragma once

#include "Tool.hpp"
#include "BackgroundGrid.hpp"
#include "Tilemap.hpp"

class MeshToolProperty : public ImageToolProperty {
protected:
    virtual void buildModalSpecifics(tgui::ScrollablePanel::Ptr& panel) override;

public:
    uint32_t tileX;
    uint32_t tileY;
    uint16_t tileValue;
    bool blocking;
    bool defaultBlocking;
    bool empty;

    virtual bool isEmpty() override { return empty; }

    virtual void clear() override {
        tileX = 0;
        tileY = 0;
        tileValue = 0;
        blocking = false;
        defaultBlocking = false;
        empty = true;
    }

    MeshToolProperty(tgui::Gui &gui, Tool* parent) : ImageToolProperty(gui, parent) { clear(); }
};

class ToolMesh : public ToolWithSprites {
public:
    enum class DrawMode {
        Pencil, RectFill, RectEdge, Line
    };

private:
    DrawMode mode = DrawMode::Pencil;
    sf::RectangleShape rectShape;

    BackgroundGrid bgr; // checker pattern for better UX
    sf::Texture texture; // tileset texture
    dgm::Clip clip; // how tileset is sliced
    Tilemap tilemap; // renders tileset
    MeshToolProperty tileProperty = MeshToolProperty(gui, this);

    std::vector<bool> defaultBlocks;

    bool drawing = false;
    sf::Vector2i penDownPos;
    sf::Vector2i penPos;

    void changeDrawingMode(DrawMode newMode);

    sf::Vector2u worldToTilePos(const sf::Vector2i& position) {
        unsigned tileX = position.x / tilemap.mesh.getVoxelSize().x;
        unsigned tileY = position.y / tilemap.mesh.getVoxelSize().y;
        return { tileX, tileY };
    }

    bool isPositionValid(const sf::Vector2u& tilePos) {
        return !(tilePos.x < 0 || tilePos.y < 0
            || tilePos.x >= tilemap.mesh.getDataSize().x
            || tilePos.y >= tilemap.mesh.getDataSize().y);
    }

    virtual tgui::Texture getSpriteAsTexture(unsigned spriteId) const override {
        return tgui::Texture(texture, clip.getFrame(spriteId));
    }

    virtual std::size_t getSpriteCount() const override {
        return clip.getFrameCount();
    }

public:
    virtual void configure(nlohmann::json &config);

    virtual void resize(unsigned width, unsigned height);

    virtual void saveTo(LevelD &lvd) override;

    virtual void loadFrom(const LevelD &lvd);

    virtual void drawTo(tgui::Canvas::Ptr &canvas, uint8_t opacity) override;

    virtual void penDown() override;

    virtual void penPosition(const sf::Vector2i &position) override;

    virtual void penUp() override;

    virtual void penCancel() override {
        drawing = false;
    }

    virtual void penDelete() override {}

    virtual ToolProperty &getProperty() override;

    virtual void setProperty(const ToolProperty &prop) override;

    virtual void buildCtxMenu(tgui::MenuBar::Ptr &menu) override;

    ToolMesh(tgui::Gui& gui) : ToolWithSprites(gui) {}
};

namespace std {
    std::string to_string(ToolMesh::DrawMode mode);
}