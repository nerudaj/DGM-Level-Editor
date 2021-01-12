#pragma once

#include <DGM/dgm.hpp>
#include <TGUI/TGUI.hpp>

class Tool;

class ToolProperty {
protected:
    Tool* parent;

    virtual void buildModalSpecifics(tgui::ScrollablePanel::Ptr& panel) = 0;

public:
    void buildModal(tgui::Gui& gui);

    virtual bool isEmpty() = 0;

    virtual void clear() = 0;

    ToolProperty(Tool* parent) : parent(parent) {}
};

class ImageToolProperty : public ToolProperty {
public:
    tgui::Texture imageTexture;

    ImageToolProperty(Tool* parent) : ToolProperty(parent) {}
};
