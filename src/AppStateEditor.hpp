#pragma once

#include <DGM/dgm.hpp>
#include <TGUI/TGUI.hpp>
#include "Camera.hpp"
#include "NewLevelDialog.hpp"
#include "LogConsole.hpp"
#include "Editor.hpp"

/**
 *  This class is responsible for drawing top level gui - topbar, canvas, console, bootstrapping
 *  space for sidebar. It is responsibility for underlying components that can fill it with content to do so.
 */
class AppStateEditor : public dgm::AppState {
private:
	// Resources
	dgm::ResourceManager resmgr;
	tgui::Theme theme;
	std::string rootDir;

	// Gui
	tgui::Gui gui;
	tgui::Canvas::Ptr canvas;
	NewLevelDialog dialogNewLevel = NewLevelDialog(gui);

	// Editor
	Editor editor = Editor(gui, theme, canvas);

	std::string savePath;

	// Build functions
	void buildLayout();
	void buildPropertiesModal(unsigned x, unsigned y, unsigned id, uint16_t flags);

	void setWindowTitle(const std::string appendix = "") {
		app->window.getWindowContext().setTitle("DGM Level Editor" + (appendix.empty() ? "" : " - " + appendix));
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
	virtual bool init() override;

	AppStateEditor(const std::string &rootDir) : rootDir(rootDir) {};
};
