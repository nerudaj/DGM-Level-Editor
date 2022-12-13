#pragma once

#include <DGM/dgm.hpp>
#include <TGUI/TGUI.hpp>
#include <json.hpp>
#include <LevelD.hpp>

#include <optional>

#include "ToolProperty.hpp"
#include "include/Commands/CommandQueue.hpp"
#include "include/Shortcuts/ShortcutEngineInterface.hpp"

struct GenericObject
{
	sf::Vector2u position;
	unsigned tag;
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
class Tool
{
private:
	virtual void buildSidebar(tgui::Gui& gui, tgui::Group::Ptr& sidebar, tgui::Theme& theme) = 0;
	std::map<sf::Keyboard::Key, std::function<void(void)>> callbacks;
	bool shiftPressed = false;

	const sf::Vector2i NULL_VECTOR = sf::Vector2i(-1, -1);
	const float DRAG_THRESHOLD = 3.f; // move by at least 3 pixels to trigger dragging
	sf::Vector2i penPos = NULL_VECTOR;
	sf::Vector2i penDownPos = NULL_VECTOR;
	bool dragging = false;

protected:
	const std::string CTX_MENU_NAME = "Tool";
	tgui::Gui& gui;
	std::function<void(void)> onStateChangedCallback;
	CommandQueue& commandQueue;
	ShortcutEngineInterface& shortcutEngine;

	void addCtxMenuItem(tgui::MenuBar::Ptr& menu, const std::string& label, std::function<void(void)> callback, sf::Keyboard::Key shortcutKey);

	[[nodiscard]]
	const sf::Vector2i& getPenPosition() const noexcept { return penPos; }

	[[nodiscard]]
	const sf::Vector2i& getPenDragStart() const noexcept { return penDownPos; }

	bool isPenDragging() const
	{
		return penDownPos != NULL_VECTOR && dgm::Math::vectorSize(sf::Vector2f(penPos - penDownPos)) > DRAG_THRESHOLD;
	};

	virtual void penClicked(const sf::Vector2i& position) = 0;
	virtual void penDragStarted(const sf::Vector2i& start)
	{
		/* this function can be unused */
	}
	virtual void penDragUpdate(const sf::Vector2i& start, const sf::Vector2i& end)
	{
		/* this function can be unused */
	}
	virtual void penDragEnded(const sf::Vector2i& start, const sf::Vector2i& end) = 0;
	virtual void penDragCancel(const sf::Vector2i& origin) = 0;

	void signalStateChanged()
	{
		onStateChangedCallback();
	}

public:
	virtual void configure(nlohmann::json& config) = 0;

	// TODO: add shrinkToFit parameter
	virtual void resize(unsigned width, unsigned height) = 0;

	virtual void saveTo(LevelD& lvd) const = 0;
	virtual void loadFrom(const LevelD& lvd) = 0;

	virtual void drawTo(tgui::Canvas::Ptr& canvas, uint8_t opacity) = 0;

	virtual void penDown() final; // When LMB is pressed
	virtual void penPosition(const sf::Vector2i& position) final; // called each frame with current mouse pos
	virtual void penUp() final; // When LMB is released

	virtual void penCancel() final; // executed when Escape is pressed
	virtual void penDelete() = 0; // executed when Del is pressed

	// Returns nullptr if no property can be returned
	[[nodiscard]]
	virtual std::unique_ptr<ToolProperty> getProperty() const = 0;
	virtual void setProperty(const ToolProperty& prop) = 0;

	virtual void buildCtxMenu(tgui::MenuBar::Ptr& menu);
	void destroyCtxMenu(tgui::MenuBar::Ptr& menu)
	{
		menu->removeMenu(CTX_MENU_NAME);
	}

	void buildSidebar(tgui::Theme& theme);

	[[nodiscard]]
	virtual std::optional<GenericObject> getHighlightedObject() const = 0;

	[[nodiscard]]
	virtual std::vector<sf::Vector2u> getPositionsOfObjectsWithTag(unsigned tag) const = 0;

	Tool(
		tgui::Gui& gui,
		std::function<void(void)> onStateChanged,
		CommandQueue& commandQueue,
		ShortcutEngineInterface& shortcutEngine)
		: gui(gui)
		, onStateChangedCallback(onStateChanged)
		, commandQueue(commandQueue)
		, shortcutEngine(shortcutEngine)
	{}
	virtual ~Tool() = default;
};

template<class T>
concept DerivedFromTool = std::derived_from<T, Tool>;

// Keeps track of which tiles were used in the past
// to display them on the right sidebar
class ToolMeshHistory
{
private:
	std::vector<unsigned> history;

public:
	void insert(unsigned value)
	{
		auto it = std::find(history.begin(), history.end(), value);
		if (it != history.end()) history.erase(it);
		history.insert(history.begin(), value);
	}

	void prune(unsigned size)
	{
		if (history.size() > size) history.resize(size);
	}

	void clear()
	{
		history.clear();
		insert(0);
	}

	std::vector<unsigned>::const_iterator begin() const
	{
		return history.begin();
	}

	std::vector<unsigned>::const_iterator end() const
	{
		return history.end();
	}
};

class ToolWithSprites : public Tool
{
protected:
	unsigned penValue = 0;
	ToolMeshHistory penHistory;

	virtual void buildSidebar(tgui::Gui& gui, tgui::Group::Ptr& sidebar, tgui::Theme& theme) override;
	void buildSpriteIdSelectionModal(tgui::Gui& gui, tgui::Theme& theme);
	void changePenValue(unsigned value, tgui::Gui& gui, tgui::Theme& theme);

public:
	virtual tgui::Texture getSpriteAsTexture(unsigned spriteId) const = 0;
	virtual std::size_t getSpriteCount() const = 0;

	ToolWithSprites(
		tgui::Gui& gui,
		std::function<void(void)> onStateChanged,
		CommandQueue& commandQueue,
		ShortcutEngineInterface& shortcutEngine)
		: Tool(gui, onStateChanged, commandQueue, shortcutEngine)
	{}
};

namespace Helper
{
	sf::Vector2i minVector(const sf::Vector2i& a, const sf::Vector2i& b);

	sf::Vector2i maxVector(const sf::Vector2i& a, const sf::Vector2i& b);
}