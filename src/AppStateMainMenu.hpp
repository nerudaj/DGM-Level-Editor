#pragma once

#include <DGM\dgm.hpp>
#include <TGUI\TGUI.hpp>
#include "Globals.hpp"

class AppStateMainMenu : public dgm::AppState {
private:
	dgm::ResourceManager resmgr;
	std::string rootDir;
	tgui::Gui gui;
	Settings settings;
	bool viewShouldBeUpdated;

	void buildLayout();

public:
	virtual void input() override;
	virtual void update() override;
	virtual void draw() override;
	virtual bool init() override;

	AppStateMainMenu(const std::string &resourcesRoot) : rootDir(resourcesRoot) {};
};