#pragma once

#include "Tool.hpp"
#include "BackgroundGrid.hpp"

class MeshToolProperty : public ToolProperty {
protected:
    virtual void buildModalSpecifics(tgui::ScrollablePanel::Ptr& panel) override;
    bool empty = false;

public:
    uint16_t tileId = 0;
    bool blocking = false;

    virtual bool isEmpty() override { return empty; }

    virtual void clear() override { empty = true; }

    MeshToolProperty(Tool* parent) : ToolProperty(parent) {}
};

class Tilemap : public dgm::TileMap {
public:
    dgm::Mesh mesh;

    unsigned getTile(unsigned tileX, unsigned tileY);

    void drawArea(const sf::Vector2i& start, const sf::Vector2i& end, bool fill, unsigned tileValue, bool blocking);
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

    unsigned penTileId = 1;
    bool drawing = false;
    sf::Vector2i penDownPos;
    sf::Vector2i penPos;

    virtual void buildSidebar(tgui::Gui &gui, tgui::Group::Ptr &sidebar, tgui::Theme &theme) override;
    void buildTileIdSelectionModal(tgui::Gui &gui);

    void changeDrawingMode(DrawMode newMode);

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