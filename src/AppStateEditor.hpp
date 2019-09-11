#pragma once

#include <DGM/dgm.hpp>
#include <TGUI/TGUI.hpp>
#include "Editor.hpp"

class AppStateEditor : public dgm::AppState {
private:
	// Resources
	dgm::ResourceManager resmgr;
	tgui::Theme theme;
	std::string rootDir;

	// Gui
	tgui::Gui gui;
	tgui::Canvas::Ptr canvas;
	
	// Editor settings
	unsigned levelWidth, levelHeight;
	float zoomLevel;
	sf::Mouse::Button lastMouseButtonPressed;
	bool drawing;
	bool properties;
	
	// Editor controller
	Editor editor;

	// Build functions
	void buildLayout();
	void buildSidebar();
	void buildSelectionModal();
	void buildNewLevelModal();
	void buildPropertiesModal(unsigned x, unsigned y, unsigned id, uint16_t flags);

	// Maintenance
	void switchEditorMode(EditorMode mode);
	void drawOnLayer();
	void newLevel();

	// Misc
	void log(const std::string& message);

public:
	// Inherited via AppState
	virtual void input() override;
	virtual void update() override;
	virtual void draw() override;
	virtual bool init() override;

	AppStateEditor(const std::string &rootDir) : rootDir(rootDir) {};
};
