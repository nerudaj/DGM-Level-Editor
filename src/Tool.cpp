#include "Tool.hpp"

void Tool::buildCtxMenu(tgui::MenuBar::Ptr& menu) {
	menu->removeMenu(CTX_MENU_NAME);
	clearShortcuts();
	menu->addMenu(CTX_MENU_NAME);
}

void Tool::buildSidebar(tgui::Theme &theme) {
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

void Tool::addCtxMenuItem(tgui::MenuBar::Ptr& menu, const std::string& label, std::function<void(void)> callback, sf::Keyboard::Key shortcutKey) {
	menu->addMenuItem(label);
	menu->connectMenuItem(CTX_MENU_NAME, label, callback);
	registerShortcut(shortcutKey, callback);
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

void ToolWithSprites::buildSidebar(tgui::Gui& gui, tgui::Group::Ptr& sidebar, tgui::Theme& theme) {
	const float SIDEBAR_WIDTH = sidebar->getSize().x;
	const float OFFSET = 10.f;
	const float BUTTON_SIZE = SIDEBAR_WIDTH - 2 * OFFSET;

	// + button
	auto addbtn = tgui::Button::create("+");
	addbtn->setRenderer(theme.getRenderer("Button"));
	addbtn->setSize(BUTTON_SIZE, BUTTON_SIZE);
	addbtn->setPosition(OFFSET, OFFSET);
	addbtn->connect("clicked", [this, &gui, &theme]() {
		buildSpriteIdSelectionModal(gui, theme);
	});
	sidebar->add(addbtn);

	// History buttons
	const unsigned BUTTON_COUNT = sidebar->getSize().y / (BUTTON_SIZE + OFFSET);
	penHistory.prune(BUTTON_COUNT);

	float yPos = 2 * OFFSET + BUTTON_SIZE;
	for (auto tileId : penHistory) {
		auto btn = tgui::Button::create();
		btn->getRenderer()->setTexture(getSpriteAsTexture(tileId));
		btn->setSize(BUTTON_SIZE, BUTTON_SIZE);
		btn->setPosition(OFFSET, yPos);
		sidebar->add(btn);

		btn->connect("clicked", [this, &gui, &theme, tileId]() { changePenValue(tileId, gui, theme); });

		/*
		if (history.getActive() != i) {
			btn->getRenderer()->setOpacity(0.2f);
		}
		*/

		yPos += OFFSET + BUTTON_SIZE;
	}
}

void ToolWithSprites::buildSpriteIdSelectionModal(tgui::Gui& gui, tgui::Theme& theme) {
	constexpr float SCROLLBAR_WIDTH = 20.f;

	if (gui.get<tgui::ChildWindow>("ToolSelection")) return;

	// Create wrapper window
	auto modal = tgui::ChildWindow::create("Tile Selection");
	modal->setSize("50%", "50%");
	modal->setPosition("25%", "25%");
	gui.add(modal, "ToolSelection");

	modal->connect("Closed", [&gui]() {
		gui.remove(gui.get<tgui::ChildWindow>("ToolSelection"));
	});

	// Create scrollable group inside of this window
	auto group = tgui::ScrollablePanel::create();
	group->getRenderer()->setScrollbarWidth(SCROLLBAR_WIDTH);
	modal->add(group);

	// Compute button widths when there are 4 buttons per row
	const unsigned BUTTONS_PER_ROW = 4;
	const float BUTTON_SIZE_OUTER = (gui.getView().getSize().x * 0.5f - SCROLLBAR_WIDTH) / BUTTONS_PER_ROW;
	const float BUTTON_MARGIN = 10.f;

	unsigned x = 0, y = 0;
	for (unsigned i = 0; i < getSpriteCount(); i++) {
		auto btn = tgui::Button::create();
		btn->getRenderer()->setTexture(getSpriteAsTexture(i));
		btn->setSize(BUTTON_SIZE_OUTER - BUTTON_MARGIN, BUTTON_SIZE_OUTER - BUTTON_MARGIN);
		btn->setPosition(x * BUTTON_SIZE_OUTER + BUTTON_MARGIN, y * BUTTON_SIZE_OUTER + BUTTON_MARGIN);

		// User chosen a particular tile
		btn->connect("pressed", [this, i, &gui, &theme]() {
			changePenValue(i, gui, theme);
			auto modal = gui.get<tgui::ChildWindow>("ToolSelection");
			modal->close();
		});

		group->add(btn);

		// Update coordinates for next button
		x++;
		if (x == BUTTONS_PER_ROW) {
			x = 0;
			y++;
		}
	}
}

void ToolWithSprites::changePenValue(unsigned value, tgui::Gui& gui, tgui::Theme& theme) {
	penValue = value;
	penHistory.insert(penValue);
	Tool::buildSidebar(theme);
}
