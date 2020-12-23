#include "Tool.hpp"

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
