#pragma once

#include <DGM/dgm.hpp>
#include <TGUI\TGUI.hpp>
#include "Globals.hpp"

class AppStateMenuOptions : public dgm::AppState {
protected:
	tgui::Gui gui;
	const dgm::ResourceManager &resmgr;
	Settings &settings;

	void createOptionLabel(tgui::Layout x, tgui::Layout y, const std::string& text);
	void buildLayout();

public:
	virtual void input() override;
	virtual void update() override;
	virtual void draw() override;
	virtual bool init() override;

	AppStateMenuOptions(const dgm::ResourceManager &resmgr, Settings &settings) : resmgr(resmgr), settings(settings) {}
};