#pragma once

#include "DialogBase.hpp"

class ResizeDialog final : public DialogInterface
{
private:
	virtual void customOpenCode() override {}

public:
	[[nodiscard]]
	unsigned getLevelWidth() const;

	[[nodiscard]]
	unsigned getLevelHeight() const;

	ResizeDialog(tgui::Gui& gui, tgui::Theme& theme);
	ResizeDialog(const ResizeDialog&) = delete;
	ResizeDialog(ResizeDialog&&) = delete;
};
