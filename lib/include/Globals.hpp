#pragma once

struct Settings
{
	float soundVolume;
	float musicVolume;
};

const float SCROLLBAR_WIDTH = 40.f;
const float TITLEBAR_HEIGHT = 50.f;

#include "Configs/Sizers.hpp"

static inline tgui::ChildWindow::Ptr createNewChildWindow(const std::string& title)
{
	auto modal = tgui::ChildWindow::create(title);
	modal->getRenderer()->setTitleBarHeight(
		Sizers::GetMenuBarHeight());
	modal->setTitleTextSize(
		Sizers::GetMenuBarTextHeight());
	return modal;
}

constexpr const char* DIALOG_TITLE_WARNING = "Warning";
constexpr const char* DIALOG_TEXT_UNSAVED_EXIT = "You have some unsaved changes. Do you want to save them before exiting?";