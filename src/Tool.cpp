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

void Tool::buildSidebar(tgui::Gui &gui, tgui::Theme &theme) {
	auto sidebar = gui.get<tgui::Group>("Sidebar");
	sidebar->removeAllWidgets();

	// Back panel of the sidebar
	auto sidebgr = tgui::Panel::create({ "100%", "100%" });
	sidebgr->setRenderer(theme.getRenderer("Panel"));
	sidebar->add(sidebgr);

	// The rest is up to actual tool
	buildSidebar(gui, sidebar, theme);
}

void Tool::registerShortcut(sf::Keyboard::Key key, std::function<void(void)> callback) {
	if (callbacks.count(key) != 0) {
		throw std::runtime_error("Cannot register callback for key " +
			std::to_string(int(key)) + ": Callback already set!");
	}

	callbacks[key] = callback;
}

void Tool::handleShortcuts(const sf::Event& event) {
	if (event.type == sf::Event::KeyPressed) {
		if (event.key.code == sf::Keyboard::LShift) shiftPressed = true;

		if (shiftPressed && callbacks.count(event.key.code) != 0) {
			callbacks.at(event.key.code)(); // execute callback;
		}
	}
	else if (event.type == sf::Event::KeyReleased) {
		if (event.key.code == sf::Keyboard::LShift) shiftPressed = false;
	}
}
