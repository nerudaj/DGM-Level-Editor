#pragma once

#include "include/Interfaces/SidebarUserInterface.hpp"
#include <LevelD.hpp>

class SidebarUserTrigger final : public SidebarUserInterface
{
public:
	using PenType = LevelD::Trigger::AreaType;

public:
	[[nodiscard]]
	SidebarUserTrigger(
		tgui::Gui& gui,
		tgui::Theme& theme) noexcept
		: SidebarUserInterface(gui, theme)
	{}

public:
	[[nodiscard]]
	constexpr PenType getPenType() const noexcept
	{
		return penType;
	}

protected: // SidebarUserInterface
	void buildSidebarInternal(tgui::Group::Ptr& sidebar) override;

protected:
	void changePen(PenType newPenType)
	{
		penType = newPenType;
		buildSidebar(); // to properly highlight active button
	}

protected:
	PenType penType = PenType::Circle;
};