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
	lvd.things = items;
}

void ToolItem::loadFrom(const LevelD& lvd) {
	items = lvd.things;
}

void ToolItem::drawTo(tgui::Canvas::Ptr& canvas, uint8_t opacity) {
	unsigned index = 0;
	sf::RectangleShape outline;
	outline.setOutlineColor(sf::Color::Red);
	outline.setOutlineThickness(2);
	outline.setFillColor(sf::Color::Transparent);

	for (auto &item : items) {
		auto& clip = renderData[item.id].clip;
		auto position = sf::Vector2f(float(int(item.x) - clip.width / 2), float(int(item.y) - clip.height / 2));
		renderData[item.id].sprite.setColor(sf::Color(255, 255, 255, opacity));
		renderData[item.id].sprite.setPosition(position);

		canvas->draw(renderData[item.id].sprite);

		// Outline for selected items
		if (selectedItems.count(index) > 0) {
			outline.setPosition(position);
			outline.setSize({ float(clip.width), float(clip.height) });
			canvas->draw(outline);
		}

		index++;
	}

	if (selecting) {
		outline.setPosition(sf::Vector2f(getSelectedAreaStart()));
		outline.setSize(sf::Vector2f(getSelectedAreaSize()));
		canvas->draw(outline);
	}
}

void ToolItem::penDown() {
	Log::write("ToolItem::penDown");
	penDownPos = penPos;

	if (!isValidPenPosForDrawing(penPos)) {
		selectedItems.clear();
		return;
	}
	if ((draggedItemId = getItemFromPosition(penPos)) != -1) {
		dragging = true;
		selectedItems.insert(draggedItemId);
		auto& item = items[draggedItemId];
		dragOffset = sf::Vector2i(item.x, item.y) - penPos;
		return;
	}
}

void ToolItem::penPosition(const sf::Vector2i& position) {
	penPos = position;

	if (not isPenDown() or not isValidPenPosForDrawing(penPos)) return;

	if (dragging) {
		moveSelectedItemsTo(penPos);
	} else if (dgm::Math::vectorSize(sf::Vector2f(penPos) - sf::Vector2f(penDownPos)) > 5) {
		selecting = true;
	}
}

void ToolItem::penUp() {
	if (not isPenDown()) return;
	Log::write("ToolItem::penUp: " + std::string(dragging ? "dragging " : " ") + std::string(selecting ? "selecting" : "") );

	selectedItems.clear();

	// If selecting then find all items that are inside selected area
	// and add them to selected
	if (selecting) {
		sf::IntRect selectedArea(getSelectedAreaStart(), getSelectedAreaSize());
		selectItemsInArea(selectedArea);
	}

	if (not dragging and not selecting) {
		addNewItem();
	}

	dragging = false;
	selecting = false;
	penDownPos = NULL_VECTOR;
}

void ToolItem::addNewItem() {
	if (not isValidPenPosForDrawing(penPos)) return;

	LevelD::Thing item;
	item.id = penValue;
	item.x = penPos.x;
	item.y = penPos.y;
	item.tag = 0;
	item.flags = 0;
	item.metadata = "";

	items.push_back(item);
	Log::write("New item pos", penPos);
}

void ToolItem::selectItemsInArea(sf::IntRect& selectedArea) {
	for (std::size_t i = 0; i < items.size(); i++) {
		sf::IntRect item(items[i].x, items[i].y, 1, 1);
		if (selectedArea.contains(int(items[i].x), int(items[i].y))) {
			selectedItems.insert(i);
		}
	}
}

void ToolItem::moveSelectedItemsTo(const sf::Vector2i& vec) {
	const sf::Vector2i offset = vec + dragOffset;
	const sf::Vector2i forward = offset - sf::Vector2i(items[draggedItemId].x, items[draggedItemId].y);

	for (auto& index : selectedItems) {
		items[index].x = dgm::Math::clamp(items[index].x + forward.x, 0, levelSize.x);
		items[index].y = dgm::Math::clamp(items[index].y + forward.y, 0, levelSize.y);
	}
}

void ToolItem::penCancel() {
	Log::write("ToolItem::penCancel");

	selectedItems.clear();

	if (dragging) {
		moveSelectedItemsTo(penDownPos);
	}

	dragging = false;
	selecting = false;
	penDownPos = NULL_VECTOR;
}

void ToolItem::penDelete() {
	unsigned less = 0;
	for (auto index : selectedItems) {
		index -= less;
		items.erase(items.begin() + index);
		less++;
	}
	selectedItems.clear();
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

void ItemToolProperty::buildModalSpecifics(tgui::ScrollablePanel::Ptr& dst) {
	constexpr bool DISABLED = false;

	addOption(dst, "Item ID:", "Unique identifier of the object", data.id, 0, DISABLED);
	addOption(dst, "X coordinate:", "Measured in pixels from top-left corner", data.x, 1);
	addOption(dst, "Y coordinate:", "Measured in pixels from top-left corner", data.y, 2);
	addOption(dst, "Tag:", "Value used to group related objects", data.tag, 3); // TODO: New tag
	addOption(dst, "Flags:", "16 bit value to alter behaviour of this object", data.flags, 4);
	addOption(dst, "Metadata:", "Text field for custom data", data.metadata, 5);
}

std::string std::to_string(ToolItem::EditMode mode) {
	if (mode == ToolItem::EditMode::ModeDraw) return "ModeDraw";
	else if (mode == ToolItem::EditMode::ModeErase) return "ModeErase";
	return "Error";
}
