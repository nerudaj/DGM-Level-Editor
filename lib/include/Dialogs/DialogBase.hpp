#pragma once

#include <TGUI/TGUI.hpp>
#include <functional>

class DialogInterface
{
public:
	struct OptionLine
	{
		std::string label;
		std::string id;
		std::string value;
		bool addHelperButton = false;
		std::function<void(void)> helperButtonCallback = [] () {};
	};

protected:
	tgui::Gui& gui;
	tgui::Theme& theme;
	const std::string DIALOG_ID;
	const std::string DIALOG_TITLE;
	const std::vector<OptionLine> OPTIONS;

protected:
	virtual void customOpenCode() = 0;

	[[nodiscard]]
	std::string getEditboxValue(const std::string& boxId) const
	{
		return gui.get<tgui::EditBox>(boxId)->getText().toAnsiString();
	}

public:
	void open(std::function<void()> confirmCallback);

	void close()
	{
		auto modal = gui.get<tgui::ChildWindow>(DIALOG_ID);
		gui.remove(modal);
	}

	[[nodiscard]]
	bool isOpen() const
	{
		return gui.get<tgui::ChildWindow>(DIALOG_ID) != nullptr;
	}

	DialogInterface(
		tgui::Gui& gui,
		tgui::Theme& theme,
		const std::string& dialogId,
		const std::string& dialogTitle,
		const std::vector<OptionLine>& options);
	DialogInterface(const DialogInterface&) = delete;
	DialogInterface(DialogInterface&&) = delete;
	virtual ~DialogInterface() {}
};
