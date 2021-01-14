#include "ToolItem.hpp"
#include "JsonHelper.hpp"
#include "LogConsole.hpp"

void ToolItem::changeEditMode(EditMode mode) {
	Log::write("Changing ToolItem editMode to " + std::to_string(mode));
	editMode = mode;
}

void ToolItem::configure(nlohmann::json& config) {
	const std::string TOOL_STR = "toolItem";

	auto items = config[TOOL_STR]["items"];
	for (auto &item : items) {
		renderData.push_back(ItemRenderData());
		auto &rd = renderData.back();
		auto texturePath = item["texture"]["path"].get<std::string>();

		if (!rd.texture.loadFromFile(texturePath)) {
			Log::write("Could not load texture: " + texturePath);
			continue;
		}

		rd.clip = JsonHelper::arrayToIntRect(item["texture"]["clip"]);
		rd.sprite.setTexture(rd.texture);
	}
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
