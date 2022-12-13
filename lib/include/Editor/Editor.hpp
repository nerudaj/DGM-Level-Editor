#pragma once

#include <DGM/dgm.hpp>
#include <TGUI/TGUI.hpp>
#include "include/Camera.hpp"
#include "include/Editor/EditorState.hpp"
#include "include/Editor/EditorStateManager.hpp"
#include "include/Tools/ToolMesh.hpp"
#include "include/Tools/ToolItem.hpp"
#include "include/Tools/ToolTrigger.hpp"
#include "include/Dialogs/ResizeLevelDialog.hpp"
#include "include/Commands/CommandQueue.hpp"
#include "include/Shortcuts/ShortcutEngine.hpp"

class EditorInterface
{
public:
	[[nodiscard]]
	virtual bool isInitialized() const noexcept = 0;

	virtual void draw() = 0;

	virtual void init(unsigned levelWidth, unsigned levelHeight, const std::string& configPath) = 0;

	virtual void handleEvent(const sf::Event& event, const sf::Vector2i& mousePos) = 0;

	[[nodiscard]]
	virtual LevelD save() const = 0;

	virtual void loadFrom(
		const LevelD& lvd,
		bool skipInit = false) = 0;

	virtual void switchTool(EditorState state) = 0;

	virtual void resizeDialog() = 0;

	virtual void resize(unsigned width, unsigned height) = 0;

	virtual void shrinkToFit() = 0;

	virtual ~EditorInterface() = default;
};

class Editor final : public EditorInterface
{
private:
	tgui::Gui& gui;
	tgui::Theme& theme;
	tgui::Canvas::Ptr& canvas;
	ResizeDialog dialog = ResizeDialog(gui, theme);
	Camera camera = Camera(canvas);
	EditorStateManager stateMgr;
	sf::CircleShape mouseIndicator;

	CommandQueue& commandQueue;
	ShortcutEngineInterface& shortcutEngine;
	std::unique_ptr<ToolProperty> currentlyOpenedProperty;

	bool initialized = false;

	bool isMouseWithinBoundaries(const sf::Vector2f& mousePos) const;

	[[nodiscard]]
	bool canScroll() const
	{
		// If property window is opened, prevent scrolling
		return gui.get<tgui::ChildWindow>("ToolPropertyModal") == nullptr;
	}

	[[nodiscard]]
	bool canOpenPropertyDialog() const
	{
		// If property window is opened, do not open new one
		return canScroll();
	}

protected:
	void populateMenuBar();

	void handleRmbClicked();

	void drawTagHighlight();

public:
	[[nodiscard]]
	virtual bool isInitialized() const noexcept override
	{
		return initialized;
	}

	virtual void handleEvent(
		const sf::Event& event,
		const sf::Vector2i& mousePos) override;

	virtual void draw() override;

	/**
	 *  Initialize Editor object with new level - it has some fixed width and height
	 *  Also there is path to config json which should be loaded and given to each
	 *  instantiated Tool.
	 */
	virtual void init(
		unsigned levelWidth,
		unsigned levelHeight,
		const std::string& configPath) override;

	virtual void switchTool(EditorState state) override;

	[[nodiscard]]
	virtual LevelD save() const override;

	virtual void loadFrom(
		const LevelD& lvd,
		bool skipInit = false) override;

	virtual void resizeDialog() override;

	virtual void resize(unsigned width, unsigned height) override;

	virtual void shrinkToFit() override;

	Editor(
		tgui::Gui& gui,
		tgui::Theme& theme,
		tgui::Canvas::Ptr& canvas,
		std::function<void(void)> onStateChanged,
		CommandQueue& commandQueue,
		ShortcutEngineInterface& shortcutEngine);
};