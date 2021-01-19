#include "ToolItem.hpp"
#include "JsonHelper.hpp"
#include "LogConsole.hpp"

void ToolItem::changeEditMode(EditMode mode) {
	Log::write("Changing ToolItem editMode to " + std::to_string(mode));
	editMode = mode;
}

std::size_t ToolItem::getItemFromPosition(const sf::Vector2i& vec) const {
	for (std::size_t i = 0; i < items.size(); i++) {
		auto clip = renderData[items[i].id].clip;
		if ((items[i].x - clip.width / 2) <= uint32_t(vec.x) &&
			(items[i].y - clip.height / 2) <= uint32_t(vec.y) &&
			(items[i].x + clip.width / 2) > uint32_t(vec.x) &&
			(items[i].y + clip.height / 2) > uint32_t(vec.y)) return i;
	}

	return -1;
}

void ToolItem::configure(nlohmann::json& config) {
	items.clear();
	renderData.clear();
	penHistory.clear();

	const std::string TOOL_STR = "toolItem";

	tileSize = JsonHelper::arrayToVector2u(config["toolMesh"]["texture"]["tileDimensions"]);

	auto items = config[TOOL_STR]["items"];
	for (auto &item : items) {
		renderData.push_back(ItemRenderData());
		auto &rd = renderData.back();
		auto texturePath = item["texture"]["path"].get<std::string>();
		rd.clip = JsonHelper::arrayToIntRect(item["texture"]["clip"]);

		if (!rd.texture.loadFromFile(texturePath)) {
			Log::write("Could not load texture: " + texturePath);
			continue;
		}
	}

	for (unsigned i = 0; i < renderData.size(); i++) {
		const auto& clip = renderData[i].clip;

		renderData[i].sprite.setTexture(renderData[i].texture);
		renderData[i].sprite.setTextureRect(clip);

		if (clip.width == clip.height) {
			renderData[i].guiTexture = tgui::Texture(renderData[i].texture, clip);
		}
		else {
			const int size = std::max(clip.width, clip.height);

			sf::Image txImg = renderData[i].texture.copyToImage();
			sf::Image sqImg;
			sqImg.create(size, size, sf::Color::Transparent);

			// Copy txImg to sqImg, centered
			unsigned xOffset = (size - clip.width) / 2;
			unsigned yOffset = (size - clip.height) / 2;
			for (unsigned y = 0; y < clip.height; y++) {
				for (unsigned x = 0; x < clip.width; x++) {
					sqImg.setPixel(x + xOffset, y + yOffset, txImg.getPixel(x + clip.left, y + clip.top));
				}
			}

			sf::Texture texture;
			texture.loadFromImage(sqImg);
			renderData[i].guiTexture = tgui::Texture(texture);
		}
	}
}

void ToolItem::resize(unsigned width, unsigned height) {
	levelSize.x = int(tileSize.x * width);
	levelSize.y = int(tileSize.y * height);
}

void ToolItem::saveTo(LevelD& lvd) {
	Log::write("ToolItem::saveTo: Not implemented");
}

void ToolItem::loadFrom(const LevelD& lvd) {
	Log::write("ToolItem::loadFrom: Not implemented");
}

void ToolItem::drawTo(tgui::Canvas::Ptr& canvas, uint8_t opacity) {
	for (auto &item : items) {
		auto& clip = renderData[item.id].clip;
		renderData[item.id].sprite.setColor(sf::Color(255, 255, 255, opacity));
		renderData[item.id].sprite.setPosition(float(item.x - clip.width / 2), float(item.y - clip.height / 2));
		canvas->draw(renderData[item.id].sprite);
	}
}

void ToolItem::penDown() {
	Log::write("ToolItem::penDown");
	penDownPos = penPos;

	if (!isValidPenPosForDrawing(penPos)) return;
	if ((draggedItemId = getItemFromPosition(penPos)) != -1) {
		dragging = true;
		auto& item = items[draggedItemId];
		dragOffset = sf::Vector2i(item.x, item.y) - penPos;
		return;
	}

	LevelD::Thing item;
	item.id = penValue;
	item.x = penPos.x;
	item.y = penPos.y;

	items.push_back(item);
	Log::write("[" + std::to_string(penPos.x) + ", " + std::to_string(penPos.y) + "]");
}

void ToolItem::penPosition(const sf::Vector2i& position) {
	penPos = position;

	if (dragging && isValidPenPosForDrawing(penPos)) {
		items[draggedItemId].x = penPos.x + dragOffset.x;
		items[draggedItemId].y = penPos.y + dragOffset.y;
	}
}

void ToolItem::penUp() {
	Log::write("ToolItem::penUp");
	dragging = false;
}

void ToolItem::penCancel() {
	Log::write("ToolItem::penCancel");
	dragging = false;
	items[draggedItemId].x = penDownPos.x + dragOffset.x;
	items[draggedItemId].y = penDownPos.y + dragOffset.y;
}

ToolProperty& ToolItem::getProperty() {
	itemProperty.clear();

	std::size_t itemId = 0;
	if ((itemId = getItemFromPosition(penPos)) != -1) {
		itemProperty.imageTexture = getSpriteAsTexture(items[itemId].id);
		itemProperty.empty = false;
		itemProperty.data = items[itemId];
		itemProperty.itemId = itemId;
	}

	return itemProperty;
}

void ToolItem::setProperty(const ToolProperty&) {
	items[itemProperty.itemId] = itemProperty.data;
}

void ToolItem::buildCtxMenu(tgui::MenuBar::Ptr& menu) {
	Tool::buildCtxMenu(menu); // bootstrap

	const std::string OPTION_DRAW = "Draw Mode (Shift+D)";
	const std::string OPTION_ERASE = "Erase Mode (Shift+E)";

	addCtxMenuItem(menu, OPTION_DRAW, [this]() { changeEditMode(EditMode::ModeDraw); }, sf::Keyboard::D);
	addCtxMenuItem(menu, OPTION_ERASE, [this]() { changeEditMode(EditMode::ModeErase); }, sf::Keyboard::E);
}

void ItemToolProperty::buildModalSpecifics(tgui::ScrollablePanel::Ptr& panel, const unsigned VERTICAL_OFFSET, const unsigned START_YPOS) {
	unsigned yOffset = START_YPOS;
	addLabel(panel, "Item ID: " + std::to_string(data.id), yOffset);

	yOffset += ROW_HEIGHT;
	addIntEdit(panel, "X coordinate: ", yOffset, data.x);

	yOffset += ROW_HEIGHT;
	addIntEdit(panel, "Y coordinate: ", yOffset, data.y);

	yOffset += ROW_HEIGHT;
	addLabel(panel, "Tag: " + std::to_string(data.tag), yOffset);

	yOffset += ROW_HEIGHT;
	addLabel(panel, "Flags: " + std::to_string(data.flags), yOffset);

	// TODO: metadata

	yOffset += ROW_HEIGHT;
	addLabel(panel, "", yOffset); // intentionaly left blank
}

std::string std::to_string(ToolItem::EditMode mode) {
	if (mode == ToolItem::EditMode::ModeDraw) return "ModeDraw";
	else if (mode == ToolItem::EditMode::ModeErase) return "ModeErase";
	return "Error";
}
