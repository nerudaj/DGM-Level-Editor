#pragma once

const float SCROLLBAR_WIDTH = 40.f;
const float TITLEBAR_HEIGHT = 50.f;

#include "Configs/Sizers.hpp"

static inline tgui::ChildWindow::Ptr createNewChildWindow(tgui::Theme& theme, const std::string& title)
{
	auto modal = tgui::ChildWindow::create(title);
	modal->setRenderer(theme.getRenderer("ChildWindow"));
	modal->getRenderer()->setTitleBarHeight(
		Sizers::GetMenuBarHeight());
	modal->setTitleTextSize(
		Sizers::GetMenuBarTextHeight());
	return modal;
}

static inline void closeModal(
	tgui::Gui& gui,
	const std::string& id)
{
	auto modal = gui.get<tgui::ChildWindow>(id);
	gui.remove(modal);
}
