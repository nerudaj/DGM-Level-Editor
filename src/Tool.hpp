#pragma once

#include <DGM/dgm.hpp>
#include <TGUI/TGUI.hpp>
#include <json.hpp>

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

/**
 *  This is generic top level class representing an Editor Tool.
 *  This is a component that can edit one attribute of LevelD -
 *  Mesh or Things or Triggers, or whatever. We don't know what the
 *  tool specifically needs, but all tools needs some initial configuration
 *  (where are sprites, how they are layed out, what are default collision
 *  values). It also needs to (de)serialize to/from LevelD, draw to screen
 *  and it always have context menu and maybe a sidebar menu (with history,
 *  possibly). We don't know how drawing works, so pen* functions update
 *  state of mouse position and whether LMB is pressed.
 *
 *  When RMB is pressed, getProperty will use current mouse pos to return
 *  pointer to pointed property (that will fire up some modal window) and
 *  when property is saved (clicking OK in modal), we can use setProperty
 *  to write it back.
 */
class Tool {
private:
    virtual void buildSidebar(tgui::Gui &gui, tgui::Group::Ptr &sidebar, tgui::Theme &theme) = 0;
    std::map<sf::Keyboard::Key, std::function<void(void)>> callbacks;
    bool shiftPressed = false;

protected:
    const std::string CTX_MENU_NAME = "Tool";

    void registerShortcut(sf::Keyboard::Key key, std::function<void(void)> callback);
    void clearShortcuts() {
        callbacks.clear();
    }

public:
    void handleShortcuts(const sf::Event& event);

	virtual void configure(nlohmann::json &config) = 0;

    virtual void resize(unsigned width, unsigned height) = 0;

	virtual void saveTo(LevelD &lvd) = 0;
	virtual void loadFrom(const LevelD &lvd) = 0;

	virtual void drawTo(tgui::Canvas::Ptr &canvas) = 0;

	virtual void penDown() = 0;
    virtual void penPosition(const sf::Vector2i &position) = 0;
	virtual void penUp() = 0;
    virtual void penCancel() = 0;

	// Returns nullptr if no property can be returned
	virtual ToolProperty &getProperty() = 0;
	virtual void setProperty(const ToolProperty &prop) = 0;

	virtual void buildCtxMenu(tgui::MenuBar::Ptr &menu) = 0;
    virtual void destroyCtxMenu(tgui::MenuBar::Ptr& menu) = 0;
	
    void buildSidebar(tgui::Gui &gui, tgui::Theme &theme);
};
