#pragma once

#include <DGM/dgm.hpp>
#include <TGUI/TGUI.hpp>
#include "Camera.hpp"
#include "Dialogs/NewLevelDialog.hpp"
#include "include/Dialogs/YesNoCancelDialog.hpp"
#include "LogConsole.hpp"
#include "include/Interfaces/EditorInterface.hpp"
#include "include/Editor/NullEditor.hpp"
#include "include/Utilities/FileApi.hpp"
#include <optional>
#include "include/Commands/CommandQueue.hpp"
#include "include/Commands/CommandHistory.hpp"
#include "include/Interfaces/ShortcutEngineInterface.hpp"
#include "include/Utilities/GC.hpp"
#include "include/Utilities/Box.hpp"

/**
 *  This class is responsible for drawing top level gui - topbar, canvas, console, bootstrapping
 *  space for sidebar. It is responsibility for underlying components that can fill it with content to do so.
 */
class AppStateEditor : public dgm::AppState
{
protected:
	// Resources
	cfg::Ini& ini;
	dgm::JsonLoader loader;
	dgm::ResourceManager resmgr = dgm::ResourceManager(loader);
	tgui::Theme theme;
	std::string rootDir;
	std::string filePath;
	std::string savePath;
	std::string configPath;
	bool unsavedChanges = false;

	// Gui
	tgui::Gui gui;
	tgui::Canvas::Ptr canvas;

	GC<CommandHistory> commandHistory;
	GC<CommandQueue> commandQueue = GC<CommandQueue>(commandHistory);
	Box<EditorInterface> editor = Box<NullEditor>();

	/*
	*  HOW TO MAKE THIS NICER:
	*  Factor out dialog builder functions to some common base.
	*  Then implement each dialog as a constructible class without any other method. This will work as "fire and forget" approach. Everything that
	* concerns a modal can be solved via lambda with tgui::Gui in capture.
	* each class has its own "submit" callback requirement and every result of
	* a modal (Yes|No|Cancel, level width/height, ...) will be passed as a parameter
	* to this callback.
	*
	* Each dialog can be then implemented via factory class (types of std::function should be swallowed and not type checked, fingers crossed)
	* and then the factory class can be injected as a dependency to this class.
	*/
	NewLevelDialog dialogNewLevel = NewLevelDialog(gui, theme, ini);
	YesNoCancelDialog dialogConfirmExit = YesNoCancelDialog(gui, theme);


	// Dependencies
	GC<ShortcutEngineInterface> shortcutEngine;
	GC<FileApiInterface> fileApi;

protected:
	void updateWindowTitle()
	{
		app.window.getWindowContext().setTitle(
			"DGM Level Editor"
			+ (filePath.empty() ? "" : " - " + filePath)
			+ (unsavedChanges ? " *" : ""));
	}

	// IO
	void newLevelDialogCallback();
	std::optional<std::string> getNewSavePath();

protected: // Build functions
	class AllowExecutionToken;

	void buildLayout();
	AllowExecutionToken buildCanvasLayout(
		const std::string& SIDEBAR_WIDTH,
		const std::string& SIDEBAR_HEIGHT,
		const std::string& TOPBAR_HEIGHT);

	class AllowExecutionToken
	{
		AllowExecutionToken() = default;
		friend AllowExecutionToken AppStateEditor::buildCanvasLayout(
			const std::string&,
			const std::string&,
			const std::string&);
	};

	void buildMenuBarLayout(
		AllowExecutionToken,
		const std::string& TOPBAR_HEIGHT,
		unsigned TOPBAR_FONT_HEIGHT);
	void buildSidebarLayout(
		AllowExecutionToken,
		const std::string& SIDEBAR_WIDTH,
		const std::string& SIDEBAR_HEIGHT,
		const std::string& TOPBAR_HEIGHT);
	tgui::ChatBox::Ptr buildLoggerLayout(
		AllowExecutionToken,
		const std::string& TOPBAR_HEIGHT,
		unsigned TOPBAR_FONT_HEIGHT);

protected: // Callback handlers
	void handleNewLevel();
	void handleLoadLevel();
	void handleSaveLevel(bool forceNewPath = false) noexcept;
	void handleUndo();
	void handleRedo();
	void handleExit(YesNoCancelDialogInterface& dialoConfirmExit);

public:
	// Inherited via AppState
	virtual void input() override;
	virtual void update() override;
	virtual void draw() override;
	virtual [[nodiscard]] bool isTransparent() const noexcept override
	{
		return false;
	}

	AppStateEditor(
		dgm::App& app,
		cfg::Ini& ini,
		const std::string& rootDir,
		GC<FileApiInterface> fileApi,
		GC<ShortcutEngineInterface> shortcutEngine);
};
