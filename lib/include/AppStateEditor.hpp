#pragma once

#include <DGM/dgm.hpp>
#include <TGUI/TGUI.hpp>
#include "Camera.hpp"
#include "Dialogs/NewLevelDialog.hpp"
#include "include/Dialogs/YesNoCancelDialog.hpp"
#include "LogConsole.hpp"
#include "Editor.hpp"
#include "include/FileApi.hpp"
#include <optional>

/**
 *  This class is responsible for drawing top level gui - topbar, canvas, console, bootstrapping
 *  space for sidebar. It is responsibility for underlying components that can fill it with content to do so.
 */
class AppStateEditor : public dgm::AppState
{
protected:
	// Resources
	cfg::Ini& ini;
	dgm::ResourceManager resmgr;
	tgui::Theme theme;
	std::string rootDir;
	std::string filePath;
	bool unsavedChanges = false;
	std::unique_ptr<FileApiInterface> fileApi;

	// Gui
	tgui::Gui gui;
	tgui::Canvas::Ptr canvas;

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
	NewLevelDialog dialogNewLevel = NewLevelDialog(gui, ini);
	YesNoCancelDialog dialogConfirmExit = YesNoCancelDialog(gui);

	// Editor
	std::unique_ptr<EditorInterface> editor;

	std::string savePath;
	std::map<sf::Keyboard::Key, std::function<void(void)>> editorShortcuts;

	// Build functions
	void buildLayout();

	void updateWindowTitle()
	{
		app.window.getWindowContext().setTitle(
			"DGM Level Editor"
			+ (filePath.empty() ? "" : " - " + filePath)
			+ (unsavedChanges ? " *" : ""));
	}

	// IO
	void newLevelDialogCallback();
	void loadLevel();
	void saveLevel(bool forceNewPath = false);

	// Shortcut
	unsigned keyShortcut = 0;

protected:
	void handleExit(YesNoCancelDialogInterface& dialoConfirmExit);
	std::optional<std::string> getNewSavePath();

public:
	// Inherited via AppState
	virtual void input() override;
	virtual void update() override;
	virtual void draw() override;
	virtual [[nodiscard]] bool isTransparent() const noexcept override
	{
		return false;
	}

	AppStateEditor(dgm::App& app, cfg::Ini& ini, const std::string& rootDir, std::unique_ptr<FileApiInterface> fileApi);
};
