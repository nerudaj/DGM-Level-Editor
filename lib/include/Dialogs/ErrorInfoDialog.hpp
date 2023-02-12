#pragma once

#include "include/Interfaces/DialogInterfaces.hpp"

#include <TGUI/TGUI.hpp>

class ErrorInfoDialog final : public ErrorInfoDialogInterface
{
public:
	ErrorInfoDialog(
		tgui::Gui& gui,
		tgui::Theme& theme) noexcept
		: gui(gui)
		, theme(theme)
	{}

public:
	virtual void open(const std::string& text) override;

private:
	tgui::Gui& gui;
	tgui::Theme& theme;
	constexpr static inline const char* DIALOG_ID = "ErrorInfoDialog";
};
