#pragma once

#include <TGUI/TGUI.hpp>

enum class UserChoice
{
	Confirmed = 0,
	Denied,
	Cancelled
};

class YesNoCancelDialogInterface
{
public:
	virtual void open(const std::string title, const std::string& text, std::function<void(UserChoice)> completedCallback) = 0;

	virtual ~YesNoCancelDialogInterface() = default;
};

class YesNoCancelDialog : public YesNoCancelDialogInterface
{
	tgui::Gui& gui;
	tgui::Theme& theme;
	const std::string DIALOG_ID = "YesNoCancelDialog";

public:
	virtual void open(const std::string title, const std::string& text, std::function<void(UserChoice)> completedCallback) override;

	YesNoCancelDialog(tgui::Gui& gui, tgui::Theme& theme) : gui(gui), theme(theme) {};
};
