#pragma once

#include "include/Interfaces/SidebarUserInterface.hpp"
#include "include/Tools/ToolPenHistory.hpp"

class SidebarUserWithSprites : public SidebarUserInterface
{
public:
	SidebarUserWithSprites(
		tgui::Gui& gui,
		tgui::Theme& theme) noexcept
		: SidebarUserInterface(gui, theme)
	{}

public:
	[[nodiscard]]
	constexpr unsigned getPenValue() const noexcept { return penValue; }

protected:
	virtual void buildSidebarInternal(tgui::Group::Ptr& sidebar) override;
	void buildSpriteIdSelectionModal();
	void changePenValue(unsigned value);

	void selectSpriteId();

	[[nodiscard]]
	virtual tgui::Texture getSpriteAsTexture(unsigned spriteId) const = 0;

	[[nodiscard]]
	virtual std::size_t getSpriteCount() const = 0;

protected:
	unsigned penValue = 0;
	ToolPenHistory penHistory;
};
