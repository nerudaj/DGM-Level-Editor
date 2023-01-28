#pragma once

#include <DGM/dgm.hpp>
#include <TGUI/TGUI.hpp>
#include "include/Editor/EditorInterface.hpp"
#include "include/Camera.hpp"
#include "include/Editor/EditorState.hpp"
#include "include/Editor/EditorStateManager.hpp"
#include "include/Tools/ToolMesh.hpp"
#include "include/Tools/ToolItem.hpp"
#include "include/Tools/ToolTrigger.hpp"
#include "include/Dialogs/ResizeLevelDialog.hpp"
#include "include/Dialogs/EditPropertyDialog.hpp"
#include "include/Commands/CommandQueue.hpp"
#include "include/Shortcuts/ShortcutEngine.hpp"
#include "include/Tools/PhysicalPen.hpp"

class Editor final : public EditorInterface
{
public:
	[[nodiscard]]
	Editor(
		tgui::Gui& gui,
		tgui::Theme& theme,
		tgui::Canvas::Ptr& canvas,
		std::function<void(void)> onStateChanged,
		GC<CommandQueue> commandQueue,
		GC<ShortcutEngineInterface> shortcutEngine);
	Editor(Editor&&) = delete;
	Editor(const Editor&) = delete;

private:
	tgui::Gui& gui;
	tgui::Theme& theme;
	tgui::Canvas::Ptr& canvas;
	ResizeDialog dialog = ResizeDialog(gui, theme);
	EditPropertyDialog editPropertyDialog = EditPropertyDialog(gui, theme);
	Camera camera = Camera(canvas);
	EditorStateManager stateMgr;
	sf::CircleShape mouseIndicator;
	PhysicalPen physicalPen;

	bool initialized = false;

private: // Dependencies
	GC<CommandQueue> commandQueue;
	GC<ShortcutEngineInterface> shortcutEngine;

private:
	[[nodiscard]]
	constexpr bool isMouseWithinBoundaries(const sf::Vector2f& mousePos) const noexcept;

	[[nodiscard]]
	bool canScroll() const
	{
		// If property window is opened, prevent scrolling
		return gui.get<tgui::ChildWindow>(EditPropertyDialog::DIALOG_ID) == nullptr;
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
	bool isInitialized() const noexcept
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


};