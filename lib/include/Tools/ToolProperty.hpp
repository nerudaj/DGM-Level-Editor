#pragma once

#include <DGM/dgm.hpp>
#include <TGUI/TGUI.hpp>

class Tool;

class PropertyTag {
private:
    uint32_t value = 0;

    PropertyTag() {}
    PropertyTag(const PropertyTag &other) = delete;
    PropertyTag(PropertyTag&& other) = delete;
    PropertyTag& operator=(const PropertyTag& other) = delete;

public:
    static PropertyTag& get() {
        static PropertyTag instance;
        return instance;
    }

    void updateTag(uint32_t val) {
        if (value < val) value = val;
    }

    uint32_t getNewTag() {
        return ++value;
    }

    ~PropertyTag() {}
};

class ToolProperty {
protected:
    using TargetPanel = tgui::ScrollablePanel::Ptr;

private:
    const sf::Color ROW_BGR_DARK = sf::Color(222, 222, 222);
    const sf::Color ROW_BGR_LIGHT = sf::Color(255, 255, 255);
    const unsigned LABEL_FONT_SIZE = 20;
    const unsigned ROW_HEIGHT = 25;
    const std::string LABEL_LEFT_MARGIN = "10%";
    const std::string LABEL_WIDTH = "40%";
    const std::string VALUE_LEFT_MARGIN = "50%";
    const std::string VALUE_WIDTH = "20%";
    const std::string TAG_LEFT_MARGIN = "70%";

    bool formValid = true;

    template<typename T>
    void addOptionUint(TargetPanel& target, const std::string& label, const std::string& tooltip, T& val, unsigned ypos, bool enabled, bool tag);

protected:
    tgui::Gui& gui;
    Tool* parent;

    virtual void buildModalSpecifics(tgui::Panel::Ptr& panel) = 0;

    tgui::Panel::Ptr getRowBackground(unsigned y, const std::string& tooltip);

    tgui::Label::Ptr getLabel(const std::string& label);

    void addOption(TargetPanel& target, const std::string& label, const std::string& tooltip, bool& val, unsigned ypos, bool enabled = true);

    void addOption(TargetPanel& target, const std::string& label, const std::string& tooltip, uint32_t& val, unsigned ypos, bool enabled = true, bool tag = false);

    void addOption(TargetPanel& target, const std::string& label, const std::string& tooltip, uint16_t& val, unsigned ypos, bool enabled = true);

    void addOption(TargetPanel& target, const std::string& label, const std::string& tooltip, std::string& val, unsigned ypos, bool enabled = true);

public:
    void buildModal();

    virtual bool isEmpty() = 0;

    virtual void clear() = 0;

    ToolProperty(tgui::Gui &gui, Tool* parent) : gui(gui), parent(parent) {}
};

class ImageToolProperty : public ToolProperty {
protected:
    virtual void buildModalSpecifics(tgui::Panel::Ptr& panel) override;

    virtual void buildModalSpecifics(tgui::ScrollablePanel::Ptr& panel) = 0;

public:
    tgui::Texture imageTexture;

    ImageToolProperty(tgui::Gui &gui, Tool* parent) : ToolProperty(gui, parent) {}
};