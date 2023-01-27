#include "include/Dialogs/EditPropertyDialog.hpp"

void EditPropertyDialog::open(
	Box<ToolPropertyInterface> property,
	ToolInterface& targetTool)
{
	currentProperty = std::move(property);

	auto modal = tgui::ChildWindow::create("Edit Properties");
	modal->setRenderer(theme.getRenderer("ChildWindow"));
	modal->setSize("50%", "80%");
	modal->setPosition("25%", "10%");
	modal->setResizable(true);
	gui.add(modal, DIALOG_ID);

	// Create scrollable group inside of the window
	auto group = tgui::Panel::create();
	group->setSize("100%", "88%");
	modal->add(group);

	currentProperty->fillEditDialog(group);

	auto submit = [&]
	{
		// TODO: if form valid
		targetTool.setProperty(*currentProperty);
		close();
	};

	buildBottomButtonBar(modal, submit);
}

void EditPropertyDialog::close()
{
	auto modal = gui.get<tgui::ChildWindow>(DIALOG_ID);
	modal->close();
	gui.remove(modal);
}

void EditPropertyDialog::buildBottomButtonBar(
	tgui::ChildWindow::Ptr& modal,
	std::function<void(void)> submit)
{
	const tgui::Layout2d BUTTON_SIZE{ "20%", "8%" };

	modal->add(createButton(
		"Ok",
		BUTTON_SIZE,
		{ "56%", "90%" },
		submit));

	modal->add(createButton(
		"Cancel",
		BUTTON_SIZE,
		{ "78%", "90%" },
		[&] { close(); }));
}

tgui::Button::Ptr EditPropertyDialog::createButton(
	const std::string& label,
	const tgui::Layout2d& size,
	const tgui::Layout2d& position,
	std::function<void(void)> callback)
{
	auto button = tgui::Button::create(label);
	button->setSize(size);
	button->setPosition(position);
	button->connect("clicked", callback);
	return button;
}

