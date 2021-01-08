#pragma once

#include "Tool.hpp"
#include "BackgroundGrid.hpp"

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

    MeshToolProperty(Tool* parent) : ImageToolProperty(parent) { clear(); }
};

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

class ToolMeshHistory {
private:
    std::vector<unsigned> history;

public:
    void insert(unsigned value) {
        auto it = std::find(history.begin(), history.end(), value);
        if (it != history.end()) history.erase(it);
        history.insert(history.begin(), value);
    }

    void prune(unsigned size) {
        if (history.size() > size) history.resize(size);
    }

    std::vector<unsigned>::const_iterator begin() const {
        return history.begin();
    }

    std::vector<unsigned>::const_iterator end() const {
        return history.end();
    }
};

class ToolMesh : public Tool {
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
    //dgm::Mesh mesh; // blocks of tileset
    MeshToolProperty tileProperty = MeshToolProperty(this);

    std::vector<bool> defaultBlocks;

    unsigned penTileId = 0;
    bool drawing = false;
    sf::Vector2i penDownPos;
    sf::Vector2i penPos;

    ToolMeshHistory penHistory;

    virtual void buildSidebar(tgui::Gui &gui, tgui::Group::Ptr &sidebar, tgui::Theme &theme) override;
    void changePenValue(unsigned value, tgui::Gui& gui, tgui::Theme& theme);
    void buildTileIdSelectionModal(tgui::Gui &gui, tgui::Theme &theme);

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

    tgui::Texture ToolMesh::getTileAsTexture(unsigned tileId);

public:
    virtual void configure(nlohmann::json &config);

    virtual void resize(unsigned width, unsigned height);

    virtual void saveTo(LevelD &lvd) override;

    virtual void loadFrom(const LevelD &lvd);

    virtual void drawTo(tgui::Canvas::Ptr &canvas) override;

    virtual void penDown() override;

    virtual void penPosition(const sf::Vector2i &position) override;

    virtual void penUp() override;

    virtual void penCancel() override {
        drawing = false;
    }

    virtual ToolProperty &getProperty() override;

    virtual void setProperty(const ToolProperty &prop) override;

    virtual void buildCtxMenu(tgui::MenuBar::Ptr &menu) override;

    virtual void destroyCtxMenu(tgui::MenuBar::Ptr& menu) override;
};

namespace std {
    std::string to_string(ToolMesh::DrawMode mode);
}