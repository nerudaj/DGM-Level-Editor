#pragma once

#include <DGM/dgm.hpp>
#include <TGUI/TGUI.hpp>

class Tool;

class ToolProperty {
protected:
    const unsigned LABEL_FONT_SIZE = 15;
    const unsigned ROW_HEIGHT = 20;
    const std::string LABEL_WIDTH = "40%";
    const std::string EDIT_WIDTH = "10%";
    const std::string LABEL_LEFT_MARGIN = "25%";
    const std::string EDIT_LEFT_MARGIN = "65%";

    tgui::Gui& gui;
    Tool* parent;

    virtual void buildModalSpecifics(tgui::ScrollablePanel::Ptr& panel) = 0;

    void addLabel(tgui::ScrollablePanel::Ptr& target, const std::string& text, unsigned y);

    void addBoolEdit(tgui::ScrollablePanel::Ptr& target, const std::string& label, unsigned y, bool &var, bool enabled = true);

    void addIntEdit(tgui::ScrollablePanel::Ptr& target, const std::string& label, unsigned y, uint32_t& var, bool enabled = true);

public:
    void buildModal();

    virtual bool isEmpty() = 0;

    virtual void clear() = 0;

    ToolProperty(tgui::Gui &gui, Tool* parent) : gui(gui), parent(parent) {}
};

class ImageToolProperty : public ToolProperty {
protected:
    virtual void buildModalSpecifics(tgui::ScrollablePanel::Ptr& panel) override;

    virtual void buildModalSpecifics(tgui::ScrollablePanel::Ptr& panel, const unsigned VERTICAL_OFFSET, const unsigned START_YPOS) = 0;

public:
    tgui::Texture imageTexture;

    ImageToolProperty(tgui::Gui &gui, Tool* parent) : ToolProperty(gui, parent) {}
};
