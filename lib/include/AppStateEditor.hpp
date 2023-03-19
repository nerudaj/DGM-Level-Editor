#pragma once

#include "include/Interfaces/DialogInterfaces.hpp"
#include "include/Interfaces/EditorInterface.hpp"
#include "include/Interfaces/FileApiInterface.hpp"
#include "include/Interfaces/ShortcutEngineInterface.hpp"

#include <DGM/dgm.hpp>
#include <Config.hpp>
#include <TGUI/TGUI.hpp>

#include "Camera.hpp"
#include "Dialogs/NewLevelDialog.hpp"
#include "Dialogs/UpdateConfigPathDialog.hpp"
#include "LogConsole.hpp"
#include "include/Editor/NullEditor.hpp"
#include <optional>
#include "include/Commands/CommandQueue.hpp"
#include "include/Commands/CommandHistory.hpp"
#include "include/Utilities/GC.hpp"
#include "include/Utilities/Box.hpp"
#include "include/Utilities/ClickPreventer.hpp"
#include "include/ProgramOptions.hpp"

class AppStateEditor : public dgm::AppState
{
protected:
	// Dependencies
	cfg::Ini& ini;
	tgui::Gui& gui;
	tgui::Theme& theme;
	GC<ShortcutEngineInterface> shortcutEngine;
	GC<FileApiInterface> fileApi;
	GC<YesNoCancelDialogInterface> dialogConfirmExit;
	GC<ErrorInfoDialogInterface> dialogErrorInfo;

	// Attributes
	dgm::JsonLoader loader;
	dgm::ResourceManager resmgr = dgm::ResourceManager(loader);
	ProgramOptions programOptions;
	std::string filePath;
	std::string savePath;
	std::optional<std::string> configPath = {};
	bool unsavedChanges = false;
	tgui::Canvas::Ptr canvas;
	GC<CommandHistory> commandHistory;
	GC<CommandQueue> commandQueue = GC<CommandQueue>(commandHistory);
	Box<EditorInterface> editor = Box<NullEditor>();
	NewLevelDialog dialogNewLevel;
	UpdateConfigPathDialog dialogUpdateConfigPath;
	ClickPreventer clickPreventer;

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
	void loadLevel(
		const std::string& pathToLevel,
		std::optional<std::string> pathToConfigOverride = {});
	void handleSaveLevel(bool forceNewPath = false) noexcept;
	void handleUndo();
	void handleRedo();
	void handleExit(YesNoCancelDialogInterface& dialoConfirmExit);

protected:
	void setupFont();

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
		tgui::Gui& gui,
		tgui::Theme& theme,
		cfg::Ini& ini,
		ProgramOptions options,
		GC<FileApiInterface> fileApi,
		GC<ShortcutEngineInterface> shortcutEngine,
		GC<YesNoCancelDialogInterface> dialogConfirmExit,
		GC<ErrorInfoDialogInterface> dialogErrorInfo);
	~AppStateEditor();
};
