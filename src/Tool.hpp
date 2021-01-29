#pragma once

#include <DGM/dgm.hpp>
#include <TGUI/TGUI.hpp>
#include <json.hpp>

#include "ToolProperty.hpp"

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
    tgui::Gui& gui;

    void registerShortcut(sf::Keyboard::Key key, std::function<void(void)> callback);
    void clearShortcuts() {
        callbacks.clear();
    }

    void addCtxMenuItem(tgui::MenuBar::Ptr& menu, const std::string &label, std::function<void(void)> callback, sf::Keyboard::Key shortcutKey);

public:
    void handleShortcuts(const sf::Event& event);

	virtual void configure(nlohmann::json &config) = 0;

    virtual void resize(unsigned width, unsigned height) = 0;

	virtual void saveTo(LevelD &lvd) = 0;
	virtual void loadFrom(const LevelD &lvd) = 0;

	virtual void drawTo(tgui::Canvas::Ptr &canvas, uint8_t opacity) = 0;

	virtual void penDown() = 0; // When LMB is pressed
    virtual void penPosition(const sf::Vector2i &position) = 0; // called each frame with current mouse pos
	virtual void penUp() = 0; // When LMB is released
    virtual void penCancel() = 0; // executed when Escape is pressed
    virtual void penDelete() = 0; // executed when Del is pressed

	// Returns nullptr if no property can be returned
	virtual ToolProperty &getProperty() = 0;
	virtual void setProperty(const ToolProperty &prop) = 0;

    virtual void buildCtxMenu(tgui::MenuBar::Ptr& menu);
    void destroyCtxMenu(tgui::MenuBar::Ptr& menu) {
        menu->removeMenu(CTX_MENU_NAME);
    }

    void buildSidebar(tgui::Theme &theme);

    Tool(tgui::Gui& gui) : gui(gui) {}
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

    void clear() {
        history.clear();
        insert(0);
    }

    std::vector<unsigned>::const_iterator begin() const {
        return history.begin();
    }

    std::vector<unsigned>::const_iterator end() const {
        return history.end();
    }
};

class ToolWithSprites : public Tool {
protected:
    unsigned penValue = 0;
    ToolMeshHistory penHistory;

    virtual void buildSidebar(tgui::Gui& gui, tgui::Group::Ptr& sidebar, tgui::Theme& theme) override;
    void buildSpriteIdSelectionModal(tgui::Gui& gui, tgui::Theme& theme);
    void changePenValue(unsigned value, tgui::Gui& gui, tgui::Theme& theme);

public:
    virtual tgui::Texture getSpriteAsTexture(unsigned spriteId) const = 0;
    virtual std::size_t getSpriteCount() const = 0;

    ToolWithSprites(tgui::Gui& gui) : Tool(gui) {}
};
