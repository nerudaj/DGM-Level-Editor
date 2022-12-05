#include "include/Dialogs/YesNoCancelDialog.hpp"
#include "include/Globals.hpp"
#include <array>
#include <utility>

static void closeModal(tgui::Gui& gui, const std::string& id)
{
	auto modal = gui.get<tgui::ChildWindow>(id);
	gui.remove(modal);
}

void YesNoCancelDialog::open(const std::string title, const std::string& text, std::function<void(UserChoice)> completedCallback)
{
	auto modal = createNewChildWindow(theme, title);
	modal->setSize("20%", "10%");
	modal->setPosition("40%", "40%");
	modal->setPositionLocked(true);
	modal->connect("EscapeKeyPressed", [this, completedCallback] ()
	{
		closeModal(gui, DIALOG_ID);
	completedCallback(UserChoice::Cancelled);
	});
	modal->connect("Closed", [this, completedCallback] ()
	{
		closeModal(gui, DIALOG_ID);
	completedCallback(UserChoice::Cancelled);
	});
	gui.add(modal, DIALOG_ID);

	auto label = tgui::Label::create(text);
	label->setSize({ "98%", "80%" });
	label->setPosition({ "1%", "1%" });
	label->setTextSize(Sizers::GetMenuBarTextHeight());
	modal->add(label);

	const tgui::Layout2d BUTTON_SIZE = { "30%", "25%" };
	const std::string BUTTON_Y_OFFSET = "72%";
	typedef std::pair<std::string, UserChoice> ButtonType;
	const std::array<ButtonType, 3> BUTTONS = {
		ButtonType{ "Yes", UserChoice::Confirmed },
		ButtonType{ "No", UserChoice::Denied },
		ButtonType{ "Cancel", UserChoice::Cancelled }
	};
	float xOffset = 2.25f;

	for (auto&& [label, choice] : BUTTONS)
	{
		auto btn = tgui::Button::create(label);
		btn->setSize(BUTTON_SIZE);
		btn->setPosition(std::to_string(xOffset) + "%", BUTTON_Y_OFFSET);
		btn->connect("clicked", [this, choice, completedCallback] ()
		{
			closeModal(gui, DIALOG_ID);
		completedCallback(choice);
		});
		modal->add(btn);
		xOffset += 32.25f;
	}
}