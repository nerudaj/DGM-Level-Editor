#include "include/Tools/Tool.hpp"
#include "include/Globals.hpp"

void Tool::buildCtxMenu(tgui::MenuBar::Ptr& menu)
{
	menu->removeMenu(CTX_MENU_NAME);
	shortcutEngine.unregisterShortcutGroup(CTX_MENU_NAME);
	menu->addMenu(CTX_MENU_NAME);
}

void Tool::buildSidebar(tgui::Theme& theme)
{
	auto sidebar = gui.get<tgui::Group>("Sidebar");
	sidebar->removeAllWidgets();

	// Back panel of the sidebar
	auto sidebgr = tgui::Panel::create({ "100%", "100%" });
	sidebgr->setRenderer(theme.getRenderer("Panel"));
	sidebar->add(sidebgr);

	// The rest is up to actual tool
	buildSidebar(gui, sidebar, theme);
}

void Tool::addCtxMenuItem(
	tgui::MenuBar::Ptr& menu,
	const std::string& label,
	std::function<void(void)> callback,
	sf::Keyboard::Key key)
{
	menu->addMenuItem(label);
	menu->connectMenuItem(CTX_MENU_NAME, label, callback);
	shortcutEngine.registerShortcut(
		CTX_MENU_NAME,
		{ false, true, key },
		callback);
}

void ToolWithSprites::buildSidebar(tgui::Gui& gui, tgui::Group::Ptr& sidebar, tgui::Theme& theme)
{
	const float SIDEBAR_WIDTH = sidebar->getSize().x;
	const float OFFSET = 10.f;
	const float BUTTON_SIZE = SIDEBAR_WIDTH - 2 * OFFSET;

	// + button
	auto addbtn = tgui::Button::create("+");
	addbtn->setRenderer(theme.getRenderer("Button"));
	addbtn->setSize(BUTTON_SIZE, BUTTON_SIZE);
	addbtn->setPosition(OFFSET, OFFSET);
	addbtn->connect("clicked", [this, &gui, &theme] ()
 {
	 buildSpriteIdSelectionModal(gui, theme);
	});
	sidebar->add(addbtn);

	// History buttons
	const unsigned BUTTON_COUNT = unsigned(sidebar->getSize().y / (BUTTON_SIZE + OFFSET));
	penHistory.prune(BUTTON_COUNT);

	float yPos = 2 * OFFSET + BUTTON_SIZE;
	for (auto tileId : penHistory)
	{
		auto btn = tgui::Button::create();
		btn->getRenderer()->setTexture(getSpriteAsTexture(tileId));
		btn->setSize(BUTTON_SIZE, BUTTON_SIZE);
		btn->setPosition(OFFSET, yPos);
		sidebar->add(btn);

		btn->connect("clicked", [this, &gui, &theme, tileId] () { changePenValue(tileId, gui, theme); });

		if (yPos != 2 * OFFSET + BUTTON_SIZE)
		{
			btn->getRenderer()->setOpacity(0.25f);
		}

		yPos += OFFSET + BUTTON_SIZE;
	}
}

void ToolWithSprites::buildSpriteIdSelectionModal(tgui::Gui& gui, tgui::Theme& theme)
{
	constexpr float SCROLLBAR_WIDTH = 40.f;

	if (gui.get<tgui::ChildWindow>("ToolSelection")) return;

	// Create wrapper window
	auto modal = createNewChildWindow(theme, "Tile Selection");
	modal->setSize("70%", "70%");
	modal->setPosition("15%", "15%");
	gui.add(modal, "ToolSelection");

	modal->connect("Closed", [&gui] ()
 {
	 gui.remove(gui.get<tgui::ChildWindow>("ToolSelection"));
	});

	// Create scrollable group inside of this window
	auto group = tgui::ScrollablePanel::create();
	group->getRenderer()->setScrollbarWidth(40.f);
	modal->add(group);

	// Compute button widths when there are 4 buttons per row
	const unsigned BUTTONS_PER_ROW = 6;
	// Must be same as string in modal->setSize
	const float MODAL_WIDTH = 0.7f;
	const float BUTTON_SIZE_OUTER = (gui.getView().getSize().x * MODAL_WIDTH - SCROLLBAR_WIDTH) / BUTTONS_PER_ROW;
	const float BUTTON_MARGIN = 10.f;

	unsigned x = 0, y = 0;
	for (unsigned i = 0; i < getSpriteCount(); i++)
	{
		auto btn = tgui::Button::create();
		btn->getRenderer()->setTexture(getSpriteAsTexture(i));
		btn->setSize(BUTTON_SIZE_OUTER - BUTTON_MARGIN, BUTTON_SIZE_OUTER - BUTTON_MARGIN);
		btn->setPosition(x * BUTTON_SIZE_OUTER + BUTTON_MARGIN, y * BUTTON_SIZE_OUTER + BUTTON_MARGIN);

		// User chosen a particular tile
		btn->connect("pressed", [this, i, &gui, &theme] ()
 {
	 changePenValue(i, gui, theme);
		auto modal = gui.get<tgui::ChildWindow>("ToolSelection");
		modal->close();
		});

		group->add(btn);

		// Update coordinates for next button
		x++;
		if (x == BUTTONS_PER_ROW)
		{
			x = 0;
			y++;
		}
	}
}

void ToolWithSprites::changePenValue(unsigned value, tgui::Gui& gui, tgui::Theme& theme)
{
	penValue = value;
	penHistory.insert(penValue);
	Tool::buildSidebar(theme);
}

void Tool::penDown()
{
	penDownPos = penPos;
}

void Tool::penPosition(const sf::Vector2i& position)
{
	penPos = position;

	if (isPenDragging())
	{
		if (dragging) penDragUpdate(penDownPos, position);
		else
		{
			dragging = true;
			penDragStarted(penDownPos);
		}
	}
}

void Tool::penUp()
{
	if (isPenDragging())
	{
		penDragEnded(penDownPos, penPos);
	}
	else
	{
		penClicked(penDownPos);
	}
	dragging = false;
	penDownPos = NULL_VECTOR;
}

void Tool::penCancel()
{
	penDragCancel(penDownPos);
	dragging = false;
	penDownPos = NULL_VECTOR;
}

sf::Vector2i Helper::minVector(const sf::Vector2i& a, const sf::Vector2i& b)
{
	return sf::Vector2i(std::min(a.x, b.x), std::min(a.y, b.y));
}

sf::Vector2i Helper::maxVector(const sf::Vector2i& a, const sf::Vector2i& b)
{
	return sf::Vector2i(std::max(a.x, b.x), std::max(a.y, b.y));
}
