#include "include/Dialogs/DialogBase.hpp"
#include "include/Globals.hpp"
#include "include/FileApi.hpp"

void DialogInterface::open(std::function<void()> confirmCallback)
{
	if (isOpen()) return;

	auto modal = createNewChildWindow(DIALOG_TITLE);
	modal->setSize("30%", "50%");
	modal->setPosition("35%", "25%");
	modal->connect("EscapeKeyPressed", [this] () { close(); });
	modal->connect("Closed", [this] () { close(); });
	gui.add(modal, DIALOG_ID);

	const auto ROW_HEIGHT = "6%";
	const auto ROW_Y_OFFSET = 2;

	for (unsigned i = 0; i < OPTIONS.size(); i++)
	{
		auto label = tgui::Label::create(OPTIONS[i].label);
		label->setSize("26%", ROW_HEIGHT);
		label->setPosition("2%", std::to_string(i * 10 + ROW_Y_OFFSET) + "%");
		label->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
		modal->add(label);
	}

	for (unsigned i = 0; i < OPTIONS.size(); i++)
	{
		auto box = tgui::EditBox::create();
		box->setSize(OPTIONS[i].addHelperButton
			? "58%"
			: "66%", ROW_HEIGHT);
		box->setPosition("32%", std::to_string(i * 10 + ROW_Y_OFFSET) + "%");
		box->setText(OPTIONS[i].value);
		modal->add(box, OPTIONS[i].id);

		if (OPTIONS[i].addHelperButton)
		{
			auto btn = tgui::Button::create("...");
			btn->setSize("8%", "6%");
			btn->setPosition("90%", "22%");
			btn->connect("clicked", OPTIONS[i].helperButtonCallback);
			modal->add(btn);
		}
	}

	auto btn = tgui::Button::create("Ok");
	btn->setSize("20%", "8%");
	btn->setPosition("56%", "90%");
	btn->connect("clicked", [this, confirmCallback] ()
	{
		confirmCallback();
		close();
	});
	modal->add(btn);

	btn = tgui::Button::create("Cancel");
	btn->setSize("20%", "8%");
	btn->setPosition("78%", "90%");
	btn->connect("clicked", [this] ()
	{
		close();
	});
	modal->add(btn);

	customOpenCode();
}

DialogInterface::DialogInterface(
	tgui::Gui& gui,
	const std::string& dialogId,
	const std::string& dialogTitle,
	const std::vector<OptionLine>& options)
	:
	gui(gui),
	DIALOG_ID(dialogId),
	DIALOG_TITLE(dialogTitle),
	OPTIONS(options)
{}
