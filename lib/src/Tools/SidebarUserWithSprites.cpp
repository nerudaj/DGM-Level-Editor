#include "include/Tools/SidebarUserWithSprites.hpp"
#include "include/Globals.hpp"

void SidebarUserWithSprites::buildSidebarInternal(tgui::Group::Ptr& sidebar)
{
	const float SIDEBAR_WIDTH = sidebar->getSize().x;
	const float OFFSET = 10.f;
	const float BUTTON_SIZE = SIDEBAR_WIDTH - 2 * OFFSET;

	// + button
	auto addbtn = tgui::Button::create("+");
	addbtn->setRenderer(theme.getRenderer("Button"));
	addbtn->setSize(BUTTON_SIZE, BUTTON_SIZE);
	addbtn->setPosition(OFFSET, OFFSET);
	addbtn->connect("clicked", [this] { buildSpriteIdSelectionModal(); });
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

		btn->connect("clicked", [this, tileId] { changePenValue(tileId); });

		if (yPos != 2 * OFFSET + BUTTON_SIZE)
		{
			btn->getRenderer()->setOpacity(0.25f);
		}

		yPos += OFFSET + BUTTON_SIZE;
	}
}

void SidebarUserWithSprites::buildSpriteIdSelectionModal()
{
	constexpr float SCROLLBAR_WIDTH = 40.f;

	if (gui.get<tgui::ChildWindow>("ToolSelection")) return;

	auto close = [this]
	{
		gui.remove(gui.get<tgui::ChildWindow>("ToolSelection"));
	};

	auto selectId = [this, close] (unsigned id)
	{
		changePenValue(id);
		close();
		//auto modal = gui.get<tgui::ChildWindow>("ToolSelection");
		//modal->close();
	};

	// Create wrapper window
	auto modal = createNewChildWindow(theme, "Tile Selection");
	modal->setSize("70%", "70%");
	modal->setPosition("15%", "15%");
	gui.add(modal, "ToolSelection");

	modal->connect("Closed", close);

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
		btn->connect("pressed", [this, i, selectId]
		{
			selectId(i);
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

void SidebarUserWithSprites::changePenValue(unsigned value)
{
	penValue = value;
	penHistory.insert(penValue);
	buildSidebar();
}
