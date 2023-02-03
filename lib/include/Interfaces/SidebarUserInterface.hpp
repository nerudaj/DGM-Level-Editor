#pragma once

#include <TGUI/TGUI.hpp>

class SidebarUserInterface
{
public:
	SidebarUserInterface(
		tgui::Gui& gui,
		tgui::Theme& theme) noexcept
		: gui(gui)
		, theme(theme)
	{}
	virtual ~SidebarUserInterface() = default;

public:
	void buildSidebar();

protected:
	virtual void buildSidebarInternal(tgui::Group::Ptr& sidebar) = 0;

protected:
	tgui::Gui& gui;
	tgui::Theme& theme;
};
