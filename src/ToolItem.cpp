#include "ToolItem.hpp"
#include "LogConsole.hpp"

void ToolItem::buildSidebar(tgui::Gui& gui, tgui::Group::Ptr& sidebar, tgui::Theme& theme) {
	const float SIDEBAR_WIDTH = sidebar->getSize().x;
	const float OFFSET = 10.f;
	const float BUTTON_SIZE = SIDEBAR_WIDTH - 2 * OFFSET;

	// + button
	auto addbtn = tgui::Button::create("+");
	addbtn->setRenderer(theme.getRenderer("Button"));
	addbtn->setSize(BUTTON_SIZE, BUTTON_SIZE);
	addbtn->setPosition(OFFSET, OFFSET);
	addbtn->connect("clicked", [this, &gui, &theme]() {
		// TODO: buildTileIdSelectionModal(gui, theme);
	});
	sidebar->add(addbtn);

	// History buttons
	/*const unsigned BUTTON_COUNT = sidebar->getSize().y / (BUTTON_SIZE + OFFSET);
	penHistory.prune(BUTTON_COUNT);

	float yPos = 2 * OFFSET + BUTTON_SIZE;
	for (auto tileId : penHistory) {
		auto btn = tgui::Button::create();
		btn->getRenderer()->setTexture(getTileAsTexture(tileId));
		btn->setSize(BUTTON_SIZE, BUTTON_SIZE);
		btn->setPosition(OFFSET, yPos);
		sidebar->add(btn);

		btn->connect("clicked", [this, &gui, &theme, tileId]() { changePenValue(tileId, gui, theme); });

		if (history.getActive() != i) {
			btn->getRenderer()->setOpacity(0.2f);
		}

		yPos += OFFSET + BUTTON_SIZE;
	}*/
}

void ToolItem::changeEditMode(EditMode mode) {
	Log::write("Changing ToolItem editMode to " + std::to_string(mode));
	editMode = mode;
}

void ToolItem::configure(nlohmann::json& config) {
	Log::write("ToolItem::configure: Not implemented");
}

void ToolItem::resize(unsigned width, unsigned height) {
	Log::write("ToolItem::resize: Not implemented");
}

void ToolItem::saveTo(LevelD& lvd) {
	Log::write("ToolItem::saveTo: Not implemented");
}

void ToolItem::loadFrom(const LevelD& lvd) {
	Log::write("ToolItem::loadFrom: Not implemented");
}

void ToolItem::drawTo(tgui::Canvas::Ptr& canvas) {
	for (auto item : items) {
		renderData[item.id].sprite.setPosition(item.x, item.y);
		canvas->draw(renderData[item.id].sprite);
	}
}

void ToolItem::penDown() {
	Log::write("ToolItem::penDown: Not implemented");
}

void ToolItem::penPosition(const sf::Vector2i& position) {
	Log::write("ToolItem::penPosition: Not implemented");
	penPos = position;
}

void ToolItem::penUp() {
	Log::write("ToolItem::penUp: Not implemented");
}

void ToolItem::penCancel() {
	Log::write("ToolItem::penCancel: Not implemented");
}

ToolProperty& ToolItem::getProperty() {
	Log::write("ToolItem::getProperty: Not implemented");
	return itemProperty;
}

void ToolItem::setProperty(const ToolProperty& prop) {
	Log::write("ToolItem::setProperty: Not implemented");
}

void ToolItem::buildCtxMenu(tgui::MenuBar::Ptr& menu) {
	Tool::buildCtxMenu(menu); // bootstrap

	const std::string OPTION_DRAW = "Draw Mode (Shift+D)";
	const std::string OPTION_ERASE = "Erase Mode (Shift+E)";

	addCtxMenuItem(menu, OPTION_DRAW, [this]() { changeEditMode(EditMode::ModeDraw); }, sf::Keyboard::D);
	addCtxMenuItem(menu, OPTION_ERASE, [this]() { changeEditMode(EditMode::ModeErase); }, sf::Keyboard::E);
}

void ItemToolProperty::buildModalSpecifics(tgui::ScrollablePanel::Ptr& panel) {
	Log::write("ToolItem::buildModalSpecifics: Not implemented");
}

bool ItemToolProperty::isEmpty() {
	Log::write("ItemToolProperty::isEmpty: Not implemented");
	return false;
}

void ItemToolProperty::clear() {
	Log::write("ItemToolProperty::clear: Not implemented");
}

std::string std::to_string(ToolItem::EditMode mode) {
	if (mode == ToolItem::EditMode::ModeDraw) return "ModeDraw";
	else if (mode == ToolItem::EditMode::ModeErase) return "ModeErase";
	return "Error";
}
