#pragma once

#include "Tool.hpp"
#include "BackgroundGrid.hpp"

class Tilemap : public dgm::TileMap {
public:
    unsigned getTile(unsigned tileX, unsigned tileY);
};

class ToolMesh : public Tool {
private:
    BackgroundGrid bgr; // checker pattern for better UX
    sf::Texture texture; // tileset texture
    dgm::Clip clip; // how tileset is sliced
    Tilemap tilemap; // renders tileset
    dgm::Mesh mesh; // blocks of tileset

    std::vector<bool> defaultBlocks;

    unsigned penTileId = 1;
    bool drawing = false;

    virtual void buildSidebar(tgui::Gui &gui, tgui::Group::Ptr &sidebar, tgui::Theme &theme) override;
    void buildTileIdSelectionModal(tgui::Gui &gui);

public:
    // Inherited via EditorTool
    virtual void configure(nlohmann::json &config);

    virtual void resize(unsigned width, unsigned height);

    virtual void saveTo(LevelD &lvd) override;

    virtual void loadFrom(const LevelD &lvd);

    virtual void drawTo(tgui::Canvas::Ptr &canvas) override;

    virtual void penDown() override;

    virtual void penPosition(const sf::Vector2i &position) override;

    virtual void penUp() override;

    virtual ToolProperty *getProperty() override;

    virtual void setProperty(const ToolProperty &prop) override;

    virtual void buildCtxMenu(tgui::Gui &gui) override;
};