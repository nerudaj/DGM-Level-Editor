#pragma once

#include <DGM/dgm.hpp>
#include <TGUI/TGUI.hpp>
#include "Camera.hpp"
#include "Dialogs/NewLevelDialog.hpp"
#include "LogConsole.hpp"
#include "Editor.hpp"

/**
 *  This class is responsible for drawing top level gui - topbar, canvas, console, bootstrapping
 *  space for sidebar. It is responsibility for underlying components that can fill it with content to do so.
 */
class AppStateEditor : public dgm::AppState {
private:
	// Resources
	cfg::Ini& ini;
	dgm::ResourceManager resmgr;
	tgui::Theme theme;
	std::string rootDir;
	std::string filePath;
	bool unsavedChanges = false;

	// Gui
	tgui::Gui gui;
	tgui::Canvas::Ptr canvas;
	NewLevelDialog dialogNewLevel = NewLevelDialog(gui, ini);

	// Editor
	Editor editor = Editor(gui, theme, canvas, [&] () {
		unsavedChanges = true;
		updateWindowTitle();
	});

	std::string savePath;
	std::map<sf::Keyboard::Key, std::function<void(void)>> editorShortcuts;

	// Build functions
	void buildLayout();

	void updateWindowTitle() {
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

public:
	// Inherited via AppState
	virtual void input() override;
	virtual void update() override;
	virtual void draw() override;
	virtual [[nodiscard]] bool isTransparent() const noexcept override {
		return false;
	}

	AppStateEditor(dgm::App& app, cfg::Ini& ini, const std::string& rootDir);
};
