#pragma once

#include "include/Interfaces/DialogInterfaces.hpp"

#include <TGUI/TGUI.hpp>

class YesNoCancelDialog : public YesNoCancelDialogInterface
{
	tgui::Gui& gui;
	tgui::Theme& theme;
	const std::string DIALOG_ID = "YesNoCancelDialog";

public:
	virtual void open(
		const std::string title,
		const std::string& text,
		std::function<void(UserChoice)> completedCallback) override;

	YesNoCancelDialog(tgui::Gui& gui, tgui::Theme& theme) : gui(gui), theme(theme) {};
};
