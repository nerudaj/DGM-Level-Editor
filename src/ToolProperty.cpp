#include "ToolProperty.hpp"
#include "Tool.hpp"

void ToolProperty::buildModal(tgui::Gui& gui) {
	const float SCROLLBAR_WIDTH = 20.f;

	// Create wrapper window
	auto modal = tgui::ChildWindow::create("Tile Properties");
	modal->setSize("50%", "80%");
	modal->setPosition("25%", "20%");
	gui.add(modal, "ToolPropertyModal");

	// Create scrollable group inside of this window
	auto group = tgui::ScrollablePanel::create();
	group->getRenderer()->setScrollbarWidth(SCROLLBAR_WIDTH);
	group->setSize("100%", "88%");
	modal->add(group);

	// Actual content
	buildModalSpecifics(group);

	// Bottom buttons
	auto close = [this, &gui]() {
		auto modal = gui.get<tgui::ChildWindow>("ToolPropertyModal");
		modal->close();
	};

	auto btn = tgui::Button::create("Ok");
	btn->setSize("20%", "8%");
	btn->setPosition("56%", "90%");
	btn->connect("clicked", [this, close]() {
		parent->setProperty(*this);
		close();
	});
	modal->add(btn);

	btn = tgui::Button::create("Cancel");
	btn->setSize("20%", "8%");
	btn->setPosition("78%", "90%");
	btn->connect("clicked", [this, close]() { close();  });
	modal->add(btn);
}