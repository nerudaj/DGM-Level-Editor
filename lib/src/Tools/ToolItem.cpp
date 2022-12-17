#include "include/Tools/ToolItem.hpp"
#include "include/JsonHelper.hpp"
#include "include/LogConsole.hpp"
#include "include/Commands/CreateDeleteObjectCommand.hpp"

#include <filesystem>

void ToolItem::changeEditMode(EditMode mode)
{
	Log::write("Changing ToolItem editMode to " + std::to_string(mode));
	editMode = mode;
}

std::size_t ToolItem::getItemFromPosition(const sf::Vector2f& vec) const
{
	for (std::size_t i = 0; i < items.size(); i++)
	{
		const auto clip = renderData[items[i].id].clip;
		const float x = float(items[i].x);
		const float y = float(items[i].y);
		const float wh = clip.width / 2.f;
		const float hh = clip.height / 2.f;
		if ((x - wh) < vec.x && vec.x < (x + wh) && (y - hh) < vec.y && vec.y < (y + hh)) return i;
	}

	return -1;
}

void ToolItem::configure(nlohmann::json& config)
{
	items.clear();
	renderData.clear();
	penHistory.clear();

	const std::string TOOL_STR = "toolItem";
	const auto rootPath = std::filesystem::path(config["configFolder"].get<std::string>());

	tileSize = JsonHelper::arrayToVector2u(config["toolMesh"]["texture"]["tileDimensions"]);

	auto items = config[TOOL_STR]["items"];
	for (auto& item : items)
	{
		renderData.push_back(ItemRenderData());
		auto& rd = renderData.back();
		auto texturePath = std::filesystem::path(item["texture"]["path"].get<std::string>());
		if (texturePath.is_relative())
			texturePath = rootPath / texturePath;

		rd.clip = JsonHelper::arrayToIntRect(item["texture"]["clip"]);

		if (!rd.texture.loadFromFile(texturePath.string()))
		{
			Log::write("Could not load texture: " + texturePath.string());
			continue;
		}
	}

	for (unsigned i = 0; i < renderData.size(); i++)
	{
		const auto& clip = renderData[i].clip;

		renderData[i].sprite.setTexture(renderData[i].texture);
		renderData[i].sprite.setTextureRect(clip);

		if (clip.width == clip.height)
		{
			renderData[i].guiTexture = tgui::Texture(renderData[i].texture, clip);
		}
		else
		{
			const int size = std::max(clip.width, clip.height);

			sf::Image txImg = renderData[i].texture.copyToImage();
			sf::Image sqImg;
			sqImg.create(size, size, sf::Color::Transparent);

			// Copy txImg to sqImg, centered
			unsigned xOffset = (size - clip.width) / 2;
			unsigned yOffset = (size - clip.height) / 2;
			for (int y = 0; y < clip.height; y++)
			{
				for (int x = 0; x < clip.width; x++)
				{
					sqImg.setPixel(x + xOffset, y + yOffset, txImg.getPixel(x + clip.left, y + clip.top));
				}
			}

			sf::Texture texture;
			texture.loadFromImage(sqImg);
			renderData[i].guiTexture = tgui::Texture(texture);
		}
	}

	selectRect.setOutlineColor(sf::Color::Red);
	selectRect.setOutlineThickness(2);
	selectRect.setFillColor(sf::Color::Transparent);
}

void ToolItem::resize(unsigned width, unsigned height)
{
	const auto newLevelSize = sf::Vector2u(width, height);
	const auto oldLevelSize = sf::Vector2u(
		levelSize.x / tileSize.x,
		levelSize.y / tileSize.y
	);

	if (oldLevelSize.x > newLevelSize.x || oldLevelSize.y > newLevelSize.y)
		return;

	const auto tileOffset = (newLevelSize - oldLevelSize) / 2u;
	const auto offset = sf::Vector2u(
		tileOffset.x * tileSize.x,
		tileOffset.y * tileSize.y
	);

	for (auto& item : items)
	{
		item.x += offset.x;
		item.y += offset.y;
	}

	levelSize.x = int(tileSize.x * width);
	levelSize.y = int(tileSize.y * height);
}

void ToolItem::saveTo(LevelD& lvd) const
{
	lvd.things = items;
}

void ToolItem::loadFrom(const LevelD& lvd)
{
	levelSize.x = lvd.mesh.layerWidth * tileSize.x;
	levelSize.y = lvd.mesh.layerHeight * tileSize.y;
	items = lvd.things;

	for (auto& item : items)
	{
		PropertyTag::get().updateTag(item.tag);
	}
}

void ToolItem::drawTo(tgui::Canvas::Ptr& canvas, uint8_t opacity)
{
	unsigned index = 0;
	sf::RectangleShape outline;
	outline.setOutlineColor(sf::Color::Red);
	outline.setOutlineThickness(2);
	outline.setFillColor(sf::Color::Transparent);

	for (auto& item : items)
	{
		auto& clip = renderData[item.id].clip;
		auto position = sf::Vector2f(float(int(item.x) - clip.width / 2), float(int(item.y) - clip.height / 2));
		renderData[item.id].sprite.setColor(sf::Color(255, 255, 255, opacity));
		renderData[item.id].sprite.setPosition(position);

		canvas->draw(renderData[item.id].sprite);

		// Outline for selected items
		if (selectedItems.count(index) > 0)
		{
			outline.setPosition(position);
			outline.setSize({ float(clip.width), float(clip.height) });
			canvas->draw(outline);
		}

		index++;
	}

	if (selecting)
	{

		//outline.setPosition(sf::Vector2f(getSelectedAreaStart()));
		//outline.setSize(sf::Vector2f(getSelectedAreaSize()));
		canvas->draw(selectRect);
	}
}

void ToolItem::selectItemsInArea(sf::IntRect& selectedArea)
{
	for (std::size_t i = 0; i < items.size(); i++)
	{
		if (selectedArea.contains(int(items[i].x), int(items[i].y)))
		{
			selectedItems.insert(i);
		}
	}
}

void ToolItem::moveSelectedItemsTo(const sf::Vector2i& vec)
{
	const sf::Vector2i offset = vec + dragOffset;
	const sf::Vector2i forward = offset - sf::Vector2i(items[draggedItemId].x, items[draggedItemId].y);

	for (auto& index : selectedItems)
	{
		items[index].x = std::clamp<unsigned>(items[index].x + forward.x, 0, levelSize.x);
		items[index].y = std::clamp<unsigned>(items[index].y + forward.y, 0, levelSize.y);
	}
}

void ToolItem::penClicked(const sf::Vector2i& position)
{
	/*
	* If pen is outside level bounds, exit
	* If pen is over item, select it, exit
	* Else create new item and clear selected
	*/

	if (not isValidPenPosForDrawing(position))
		return;

	const std::size_t itemId = getItemFromPosition(
		sf::Vector2f(position));
	if (itemId != -1)
	{
		selectedItems.insert(itemId);
		return;
	}

	const auto itemToCreate = LevelD::Thing{
		.id = penValue,
		.tag = 0,
		.x = static_cast<uint32_t>(position.x),
		.y = static_cast<uint32_t>(position.y),
		.flags = 0,
		.metadata = ""
	};

	commandQueue.push<CreateItemCommand>(
		items,
		itemToCreate);

	selectedItems.clear();
}

void ToolItem::penDragStarted(const sf::Vector2i& start)
{
	Log::write("penDragStarted");
	/* If pen is over item, then select it and commence dragging
	*	If multiple items are selected and this item was not, unselect
	*   all and move only this object
	*  Else selecting
	*/

	if ((draggedItemId = getItemFromPosition(sf::Vector2f(start))) != -1)
	{
		dragging = true;
		if (selectedItems.count(draggedItemId) == 0) selectedItems.clear();
		selectedItems.insert(draggedItemId);
		auto& item = items[draggedItemId];
		dragOffset = sf::Vector2i(item.x, item.y) - start;
	}
	else
	{
		selectRect.setPosition(sf::Vector2f(start));
		selecting = true;
	}
}

void ToolItem::penDragUpdate(const sf::Vector2i& start, const sf::Vector2i& end)
{
	/* If dragging, move all items to new position
	*/
	if (dragging && isValidPenPosForDrawing(end))
	{
		moveSelectedItemsTo(end);
	}

	if (selecting)
	{
		selectRect.setSize(sf::Vector2f(end - start));
	}
}

void ToolItem::penDragEnded(const sf::Vector2i& start, const sf::Vector2i& end)
{
	signalStateChanged();

	Log::write("penDragEnded");
	/* If selecting, select all items in area (clear everything previously selected)
	*  Clear dragging and selecting flags
	*/
	if (selecting)
	{
		selectedItems.clear();
		sf::IntRect selectedArea(
			Helper::minVector(start, end),
			{ std::abs(start.x - end.x), std::abs(start.y - end.y) });
		selectItemsInArea(selectedArea);
	}

	if (dragging)
	{
		// TODO: command
	}

	selecting = false;
	dragging = false;
}

void ToolItem::penDragCancel(const sf::Vector2i& origin)
{
	selectedItems.clear();

	if (dragging)
	{
		moveSelectedItemsTo(origin);
	}

	dragging = false;
	selecting = false;
}

void ToolItem::penDelete()
{
	if (selectedItems.empty())
		return;

	const auto idsToDelete = std::vector<std::size_t>(
		selectedItems.begin(),
		selectedItems.end());

	commandQueue.push<DeleteItemCommand>(
		items,
		idsToDelete);
	selectedItems.clear();
}

std::unique_ptr<ToolProperty> ToolItem::getProperty() const
{
	auto&& pos = getPenPosition();

	const std::size_t itemId = getItemFromPosition(sf::Vector2f(pos));
	if (itemId == -1)
		return nullptr;

	auto&& result = std::make_unique<ItemToolProperty>();

	result->imageTexture = getSpriteAsTexture(items[itemId].id);
	result->data = items[itemId];
	result->itemId = itemId;

	return std::move(result);
}

void ToolItem::setProperty(const ToolProperty& prop)
{
	auto&& property = dynamic_cast<const ItemToolProperty&>(prop);

	items[property.itemId] = property.data;
	items[property.itemId].x = std::clamp<unsigned>(
		items[property.itemId].x,
		0u,
		levelSize.x);
	items[property.itemId].y = std::clamp<unsigned>(
		items[property.itemId].y,
		0u,
		levelSize.y);

	PropertyTag::get().updateTag(items[property.itemId].tag);

	// FIXME: Command
	signalStateChanged();
}

void ToolItem::buildCtxMenu(tgui::MenuBar::Ptr& menu)
{
	Tool::buildCtxMenu(menu); // bootstrap

	const std::string OPTION_DRAW = "Draw Mode (Shift+D)";
	const std::string OPTION_ERASE = "Erase Mode (Shift+E)";

	addCtxMenuItem(menu, OPTION_DRAW, [this] { changeEditMode(EditMode::ModeDraw); }, sf::Keyboard::D);
	addCtxMenuItem(menu, OPTION_ERASE, [this] { changeEditMode(EditMode::ModeErase); }, sf::Keyboard::E);
}

std::optional<GenericObject> ToolItem::getHighlightedObject() const
{
	auto&& prop = getProperty();
	if (!prop)
		return {};

	auto&& itemProp = dynamic_cast<ItemToolProperty&>(*prop);
	return GenericObject{
		.position = { itemProp.data.x, itemProp.data.y },
		.tag = itemProp.data.tag
	};
}

std::vector<sf::Vector2u> ToolItem::getPositionsOfObjectsWithTag(unsigned tag) const
{
	std::vector<sf::Vector2u> result;
	for (auto&& item : items)
	{
		if (item.tag == tag)
			result.push_back({ item.x, item.y });
	}

	return result;
}

void ItemToolProperty::buildModalSpecifics(tgui::Gui& gui, tgui::ScrollablePanel::Ptr& dst)
{
	constexpr bool DISABLED = false;

	addOption(gui, dst, "Item ID:", "Unique identifier of the object", data.id, 0, DISABLED);
	addOption(gui, dst, "X coordinate:", "Measured in pixels from top-left corner", data.x, 1);
	addOption(gui, dst, "Y coordinate:", "Measured in pixels from top-left corner", data.y, 2);
	addOption(gui, dst, "Tag:", "Value used to group related objects", data.tag, 3, true, true);
	addOption(gui, dst, "Flags:", "16 bit value to alter behaviour of this object", data.flags, 4);
	addOption(dst, "Metadata:", "Text field for custom data", data.metadata, 5);
}

std::string std::to_string(ToolItem::EditMode mode)
{
	if (mode == ToolItem::EditMode::ModeDraw) return "ModeDraw";
	else if (mode == ToolItem::EditMode::ModeErase) return "ModeErase";
	return "Error";
}
