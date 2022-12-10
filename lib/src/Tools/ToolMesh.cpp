#include "include/Tools/ToolMesh.hpp"
#include "include/JsonHelper.hpp"
#include "include/LogConsole.hpp"
#include <filesystem>
#include "include/Commands/SetTileCommand.hpp"
#include "include/Commands/SetTileAreaCommand.hpp"
#include <include/Commands/ResizeCommand.hpp>
#include "include/Utilities/Utilities.hpp"

void ToolMesh::penClicked(const sf::Vector2i& position)
{
	signalStateChanged(); // those signals could be handled by commands

	const auto tilePos = worldToTilePos(position);
	if (isPositionValid(tilePos) && penValue != map.getTileValue(tilePos))
	{
		commandQueue.push<SetTileCommand>(
			map,
			tilePos,
			penValue,
			defaultBlocks.at(penValue));
	}
}

void ToolMesh::penDragStarted(const sf::Vector2i& start)
{
	rectShape.setPosition(sf::Vector2f(start));
}

void ToolMesh::penDragUpdate(const sf::Vector2i& start, const sf::Vector2i& end)
{
	if (mode == DrawMode::Pencil)
	{
		penClicked(end);
	}
	else
	{
		rectShape.setSize(sf::Vector2f(end - start));
	}
}

void ToolMesh::penDragEnded(const sf::Vector2i& start, const sf::Vector2i& end)
{
	signalStateChanged();

	const auto startTile = worldToTilePos(
		Utilities::clipNegativeCoords(start));
	const auto endTile = worldToTilePos(
		Utilities::clipNegativeCoords(end));
	if (mode == DrawMode::RectEdge)
	{
		constexpr bool DONT_FILL = false;
		commandQueue.push<SetTileAreaCommand>(
			map,
			startTile,
			endTile,
			penValue,
			defaultBlocks.at(penValue),
			DONT_FILL);
	}
	else if (mode == DrawMode::RectFill)
	{
		constexpr bool FILL = true;
		commandQueue.push<SetTileAreaCommand>(
			map,
			startTile,
			endTile,
			penValue,
			defaultBlocks.at(penValue),
			FILL);
	}
}

void ToolMesh::penDragCancel(const sf::Vector2i&) {}

void ToolMesh::configure(nlohmann::json& config)
{
	const std::string TOOL_STR = "toolMesh";
	const auto rootPath = std::filesystem::path(config["configFolder"].get<std::string>());

	auto texturePath = std::filesystem::path(config[TOOL_STR]["texture"]["path"].get<std::string>());
	if (texturePath.is_relative())
		texturePath = rootPath / texturePath;

	if (!texture.loadFromFile(texturePath.string()))
		throw dgm::ResourceException("Cannot load texture file: " + texturePath.string());

	sf::Vector2u tileDims = JsonHelper::arrayToVector2u(config[TOOL_STR]["texture"]["tileDimensions"]);
	sf::Vector2u tileOffs = JsonHelper::arrayToVector2u(config[TOOL_STR]["texture"]["tileOffsets"]);
	sf::IntRect  bounds = JsonHelper::arrayToIntRect(config[TOOL_STR]["texture"]["boundaries"]);

	defaultBlocks.resize(config[TOOL_STR]["defaultProperties"]["count"]);
	unsigned i = 0;
	for (auto& item : config[TOOL_STR]["defaultProperties"]["solids"])
	{
		defaultBlocks[i++] = bool(int(item));
		Log::write(std::to_string(i) + ": " + std::to_string(bool(int(item))));
	}

	clip = dgm::Clip(tileDims, bounds, 0, tileOffs);
	map = DrawableLeveldMesh(texture, clip); // NOTE: The copy probly messes up internal texture (shared_ptr?)

	rectShape.setOutlineColor(sf::Color(255, 0, 0, 128));
	rectShape.setOutlineThickness(2.f);

	penHistory.clear();
}

void ToolMesh::resize(unsigned width, unsigned height)
{
	signalStateChanged();

	ResizeMeshCommand command(map, { width, height });
	command.exec();

	// TODO: Testing scenario:
	// resizing (bigger, smaller)
}

void ToolMesh::saveTo(LevelD& lvd)
{
	Log::write("ToolMesh::saveTo");
	lvd.mesh.tileWidth = clip.getFrameSize().x;
	lvd.mesh.tileHeight = clip.getFrameSize().y;
	lvd.mesh.layerWidth = map.getMapDimensions().x;
	lvd.mesh.layerHeight = map.getMapDimensions().y;

	lvd.mesh.layers.resize(1);
	lvd.mesh.layers[0].tiles.resize(lvd.mesh.layerWidth * lvd.mesh.layerHeight);
	lvd.mesh.layers[0].blocks.resize(lvd.mesh.layerWidth * lvd.mesh.layerHeight);
	for (unsigned tileY = 0, i = 0; tileY < lvd.mesh.layerHeight; tileY++)
	{
		for (unsigned tileX = 0; tileX < lvd.mesh.layerWidth; tileX++, i++)
		{
			lvd.mesh.layers[0].tiles[i] = map.getTileValue({ tileX, tileY });
			lvd.mesh.layers[0].blocks[i] = map.isTileSolid({ tileX, tileY });
		}
	}
}

void ToolMesh::loadFrom(const LevelD& lvd)
{
	Log::write("ToolMesh::loadFrom");

	const auto tilemapData = std::vector<int>(
		lvd.mesh.layers[0].tiles.begin(),
		lvd.mesh.layers[0].tiles.end());
	const auto collisionData = std::vector<int>(
		lvd.mesh.layers[0].blocks.begin(),
		lvd.mesh.layers[0].blocks.end());
	const auto dataSize = sf::Vector2u(lvd.mesh.layerWidth, lvd.mesh.layerHeight);

	map.build(tilemapData, collisionData, dataSize);
}

void ToolMesh::drawTo(tgui::Canvas::Ptr& canvas, uint8_t)
{
	map.drawTo(canvas, enableOverlay);

	if (isPenDragging())
	{
		if (mode == DrawMode::RectEdge || mode == DrawMode::RectFill)
		{
			canvas->draw(rectShape);
		}
	}
}

std::unique_ptr<ToolProperty> ToolMesh::getProperty() const
{
	auto pos = getPenPosition();

	auto tilePos = worldToTilePos(pos);
	if (not isPositionValid(tilePos))
		return nullptr;

	auto&& result = std::make_unique<MeshToolProperty>();

	result->tileX = tilePos.x;
	result->tileY = tilePos.y;
	result->tileValue = map.getTileValue(tilePos);
	result->blocking = map.isTileSolid(tilePos);
	result->defaultBlocking = defaultBlocks[result->tileValue];
	result->imageTexture = getSpriteAsTexture(result->tileValue);

	return std::move(result);
}

void ToolMesh::setProperty(const ToolProperty& prop)
{
	auto&& property = dynamic_cast<const MeshToolProperty&>(prop);

	map.setTileValue({ property.tileX, property.tileY }, property.tileValue);
	map.setTileValue({ property.tileX, property.tileY }, property.blocking);

	signalStateChanged();
	// TODO: command
}

void ToolMesh::buildCtxMenu(tgui::MenuBar::Ptr& menu)
{
	Tool::buildCtxMenu(menu); // bootstrap

	const std::string OPTION_PENCIL = "Pencil Mode (Shift+P)";
	const std::string OPTION_FILL = "Rect-fill Mode (Shift+F)";
	const std::string OPTION_EDGE = "Rect-edge Mode (Shift+E)";
	const std::string OPTION_OVERLAY = "Toggle overlay (Shift+O)";

	addCtxMenuItem(menu, OPTION_PENCIL, [this] () { changeDrawingMode(DrawMode::Pencil); }, sf::Keyboard::P);
	addCtxMenuItem(menu, OPTION_FILL, [this] () { changeDrawingMode(DrawMode::RectFill); }, sf::Keyboard::F);
	addCtxMenuItem(menu, OPTION_EDGE, [this] () { changeDrawingMode(DrawMode::RectEdge); }, sf::Keyboard::E);
	addCtxMenuItem(menu, OPTION_OVERLAY, [this] () { enableOverlay = !enableOverlay; }, sf::Keyboard::O);
}

void ToolMesh::changeDrawingMode(ToolMesh::DrawMode newMode)
{
	Log::write("Mesh: Changing drawTo mode to " + std::to_string(newMode));
	mode = newMode;
	if (mode == DrawMode::RectEdge)
	{
		rectShape.setFillColor(sf::Color::Transparent);
	}
	else if (mode == DrawMode::RectFill)
	{
		rectShape.setFillColor(sf::Color(128, 0, 0, 128));
	}
}

std::string std::to_string(ToolMesh::DrawMode mode)
{
	if (mode == ToolMesh::DrawMode::Pencil) return "Pencil";
	else if (mode == ToolMesh::DrawMode::RectFill) return "RectFill";
	else if (mode == ToolMesh::DrawMode::RectEdge) return "RectEdge";
	else if (mode == ToolMesh::DrawMode::Line) return "Line";
	return "Error";
}

void MeshToolProperty::buildModalSpecifics(tgui::Gui& gui, tgui::ScrollablePanel::Ptr& dst)
{
	constexpr bool DISABLED = false; // the function accepts predicate enabled

	addOption(gui, dst, "Tile X:", "X coordinate of the tile", tileX, 0, DISABLED);
	addOption(gui, dst, "Tile Y:", "Y coordinate of the tile", tileY, 1, DISABLED);
	addOption(dst, "Impassable:", "Whether this tile blocks the player", blocking, 2);
	addOption(dst, "Impassable by default:", "Whether this type of tile is impassable by default", defaultBlocking, 3, DISABLED);
}
