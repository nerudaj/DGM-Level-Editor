#include "include/Tools/ToolItem.hpp"
#include "include/JsonHelper.hpp"
#include "include/LogConsole.hpp"
#include "include/Commands/CommandHelper.hpp"
#include "include/Commands/CreateDeleteObjectCommand.hpp"
#include "include/Commands/MoveObjectCommand.hpp"

#include <filesystem>

/* Implementing ToolWithDragAndSelect */
std::optional<std::size_t> ToolItem::getObjectIndexFromMousePos(const sf::Vector2i& pos) const
{
	for (std::size_t i = 0; i < items.size(); i++)
	{
		auto&& clip = sidebarUser.getClip(items[i].id);
		const float x = float(items[i].x);
		const float y = float(items[i].y);
		const float wh = clip.width / 2.f;
		const float hh = clip.height / 2.f;
		if ((x - wh) < pos.x && pos.x < (x + wh) && (y - hh) < pos.y && pos.y < (y + hh)) return i;
	}

	return {};
}

sf::Vector2i ToolItem::getPositionOfObjectWithIndex(std::size_t index) const
{
	return sf::Vector2i(
		items.at(index).x,
		items.at(index).y);
}

void ToolItem::selectObjectsInArea(const sf::IntRect& selectedArea)
{
	for (std::size_t i = 0; i < items.size(); i++)
	{
		if (selectedArea.contains(int(items[i].x), int(items[i].y)))
		{
			selectedObjects.insert(i);
		}
	}
}
void ToolItem::moveSelectedObjectsTo(const sf::Vector2i& point)
{
	CommandHelper::moveSelectedObjectsTo(
			items,
			selectedObjects,
			dragContext,
			point,
			sf::Vector2u(levelSize));
}

void ToolItem::createMoveCommand(
	const sf::Vector2i& src,
	const sf::Vector2i& dest)
{
	commandQueue.push<MoveItemCommand>(
		items,
		selectedObjects,
		dragContext,
		src,
		dest,
		sf::Vector2u(levelSize));
}

void ToolItem::createDeleteCommand()
{
	commandQueue.push<DeleteItemCommand>(
			items,
			std::vector<std::size_t>(
				selectedObjects.begin(),
				selectedObjects.end()));
}

/* Rest of ToolItem */

void ToolItem::changeEditMode(EditMode mode)
{
	Log::write("Changing ToolItem editMode to " + std::to_string(mode));
	editMode = mode;
}

void ToolItem::configure(nlohmann::json& config)
{
	items.clear();

	const std::string TOOL_STR = "toolItem";
	const auto rootPath = std::filesystem::path(config["configFolder"].get<std::string>());

	tileSize = JsonHelper::arrayToVector2u(config["toolMesh"]["texture"]["tileDimensions"]);

	std::vector<SidebarUserItem::PathRectPair> pathRectPairs;
	auto itemsJ = config[TOOL_STR]["items"];
	pathRectPairs.reserve(itemsJ.size());

	for (auto&& item : itemsJ)
	{
		auto texturePath = std::filesystem::path(item["texture"]["path"].get<std::string>());
		if (texturePath.is_relative())
			texturePath = rootPath / texturePath;

		pathRectPairs.push_back(SidebarUserItem::PathRectPair{
			.texturePath = texturePath,
			.clip = JsonHelper::arrayToIntRect(item["texture"]["clip"])
		});
	}

	sidebarUser.configure(pathRectPairs);

	selectMarker.setOutlineColor(sf::Color::Red);
	selectMarker.setOutlineThickness(2);
	selectMarker.setFillColor(sf::Color::Transparent);
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

	for (auto& item : items)
	{
		sidebarUser.drawSprite(
			canvas,
			item.id,
			sf::Vector2i(item.x, item.y),
			selectedObjects.contains(index),
			opacity);

		index++;
	}

	if (selecting)
	{
		canvas->draw(selectMarker);
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

	const auto itemId = getObjectIndexFromMousePos(position);
	if (itemId.has_value())
	{
		selectedObjects.insert(*itemId);
		return;
	}

	const auto itemToCreate = LevelD::Thing{
		.id = sidebarUser.getPenValue(),
		.tag = 0,
		.x = static_cast<uint32_t>(position.x),
		.y = static_cast<uint32_t>(position.y),
		.flags = 0,
		.metadata = ""
	};

	commandQueue.push<CreateItemCommand>(
		items,
		itemToCreate);

	selectedObjects.clear();
}

ExpectedPropertyPtr ToolItem::getProperty(const sf::Vector2i& penPos) const
{
	const auto itemId = getObjectIndexFromMousePos(penPos);
	if (!itemId)
		return std::unexpected(BaseError());

	auto&& result = Box<ItemToolProperty>();

	result->imageTexture = sidebarUser.getSpriteAsTexture(items.at(*itemId).id);
	result->data = items.at(*itemId);
	result->itemId = *itemId;

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

void ToolItem::buildCtxMenuInternal(tgui::MenuBar::Ptr& menu)
{
	const std::string OPTION_DRAW = "Draw Mode (Shift+D)";
	const std::string OPTION_ERASE = "Erase Mode (Shift+E)";

	addCtxMenuItem(menu, OPTION_DRAW, [this] { changeEditMode(EditMode::ModeDraw); }, sf::Keyboard::D);
	addCtxMenuItem(menu, OPTION_ERASE, [this] { changeEditMode(EditMode::ModeErase); }, sf::Keyboard::E);
}

std::optional<GenericObject> ToolItem::getHighlightedObject(const sf::Vector2i& penPos) const
{
	auto&& prop = getProperty(penPos);
	if (!prop)
		return {};

	auto&& itemProp = dynamic_cast<ItemToolProperty&>(
		*(prop.value()));
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
