#include "LayerTool.hpp"
#include <iostream>

void LayerToolTile::init(const nlohmann::json& config) {
	auto& defaultPropsConfig = config["layerTile"]["defaultProperties"];
	const unsigned propertyCount = defaultPropsConfig["count"];

	propDb.clear();
	selectedIndex = 0;

	propDb.reserve(propertyCount);

	for (unsigned i = 0; i < propertyCount; i++) {
		bool solid = defaultPropsConfig["solids"][i] > 0;
		propDb.push_back(LayerPropertyTile(i, solid));
	}
}

void LayerTool::buildPropertySelectionModal(tgui::Gui& gui) {
	const float SCROLLBAR_WIDTH = 20.f;

	// Create wrapper window
	auto modal = tgui::ChildWindow::create("Tool Selection");
	modal->setSize("50%", "50%");
	modal->setPosition("25%", "25%");
	gui.add(modal, "ToolSelection");

	// Create scrollable group inside of this window
	auto group = tgui::ScrollablePanel::create();
	group->getRenderer()->setScrollbarWidth(SCROLLBAR_WIDTH);
	modal->add(group);

	// Compute button widths when there are 4 buttons per row
	const unsigned BUTTONS_PER_ROW = 4;
	const float BUTTON_SIZE_OUTER = (gui.getView().getSize().x * 0.5f - SCROLLBAR_WIDTH) / BUTTONS_PER_ROW;
	const float BUTTON_MARGIN = 10.f;

	unsigned x = 0, y = 0;
	for (unsigned i = 0; i < getPropertyCount(); i++) {
		auto btn = tgui::Button::create();
		btn->getRenderer()->setTexture(getPropertyThumbnail(i));
		btn->setSize(BUTTON_SIZE_OUTER - BUTTON_MARGIN, BUTTON_SIZE_OUTER - BUTTON_MARGIN);
		btn->setPosition(x * BUTTON_SIZE_OUTER + BUTTON_MARGIN, y * BUTTON_SIZE_OUTER + BUTTON_MARGIN);

		btn->connect("pressed", [this, i, &gui]() {
			auto modal = gui.get<tgui::ChildWindow>("ToolSelection");
			modal->close();
			selectProperty(i);
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
