#include "include/Tools/ToolMesh.hpp"
#include "include/JsonHelper.hpp"
#include "include/Commands/SetTileCommand.hpp"
#include "include/Commands/SetTileAreaCommand.hpp"
#include "include/Commands/SetTilePropertyCommand.hpp"
#include "include/Commands/ResizeCommand.hpp"
#include "include/Utilities/Utilities.hpp"
#include "include/Dialogs/DialogBuilderHelper.hpp"

#include <filesystem>

void ToolMesh::penClicked(const sf::Vector2i& position)
{
	const auto tilePos = worldToTilePos(position);
	if (isPositionValid(tilePos) && sidebarUser.getPenValue() != map.getTileValue(tilePos))
	{
		commandQueue->push<SetTileCommand>(
			map,
			tilePos,
			sidebarUser.getPenValue(),
			defaultBlocks.at(sidebarUser.getPenValue()));
	}
}

void ToolMesh::penDragStarted(const sf::Vector2i& start)
{
	dragging = true;
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
	dragging = true;

	const auto startTile = worldToTilePos(
		Utilities::clipNegativeCoords(start));
	const auto endTile = worldToTilePos(
		Utilities::clipNegativeCoords(end));
	if (mode == DrawMode::RectEdge)
	{
		constexpr bool DONT_FILL = false;
		commandQueue->push<SetTileAreaCommand>(
			map,
			startTile,
			endTile,
			sidebarUser.getPenValue(),
			defaultBlocks.at(sidebarUser.getPenValue()),
			DONT_FILL);
	}
	else if (mode == DrawMode::RectFill)
	{
		constexpr bool FILL = true;
		commandQueue->push<SetTileAreaCommand>(
			map,
			startTile,
			endTile,
			sidebarUser.getPenValue(),
			defaultBlocks.at(sidebarUser.getPenValue()),
			FILL);
	}
}

void ToolMesh::configure(nlohmann::json& config)
{
	const std::string TOOL_STR = "toolMesh";
	const auto rootPath = std::filesystem::path(
		config["configFolder"].get<std::string>());

	auto texturePath = std::filesystem::path(
		config[TOOL_STR]["texture"]["path"].get<std::string>());
	if (texturePath.is_relative())
		texturePath = rootPath / texturePath;

	sf::Vector2u tileDims = JsonHelper::arrayToVector2u(
		config[TOOL_STR]["texture"]["tileDimensions"]);
	sf::Vector2u tileOffs = JsonHelper::arrayToVector2u(
		config[TOOL_STR]["texture"]["tileOffsets"]);
	sf::IntRect  bounds = JsonHelper::arrayToIntRect(
		config[TOOL_STR]["texture"]["boundaries"]);

	std::vector<bool> blocks(
		config[TOOL_STR]["defaultProperties"]["count"],
		false);

	unsigned i = 0;
	for (auto& item : config[TOOL_STR]["defaultProperties"]["solids"])
	{
		blocks[i++] = bool(int(item));
	}

	configure(
		texturePath,
		tileDims,
		tileOffs,
		bounds,
		blocks);
}

void ToolMesh::copySourceRectToTarget(
	sf::Vector2u const& start,
	sf::Vector2u const& end,
	sf::Vector2i const& translation,
	std::vector<int>& targetTileValues,
	std::vector<int>& targetSolidValues,
	unsigned targetWidth)
{
	for (unsigned y = start.y; y < end.y; y++)
	{
		for (unsigned x = start.x; x < end.x; x++)
		{
			const unsigned targetX = x + translation.x;
			const unsigned targetY = y + translation.y;
			const unsigned targetI = targetY * targetWidth + targetX;

			targetTileValues[targetI] = map.getTileValue({ x, y });
			targetSolidValues[targetI] = map.isTileSolid({ x, y });
		}
	}
}

void ToolMesh::resize(unsigned width, unsigned height)
{
	auto tileValues = std::vector<int>(width * height, 0);
	auto solidValues = std::vector<int>(width * height, 0);

	const bool upscalingX = width > map.getMapDimensions().x;
	const bool upscalingY = height > map.getMapDimensions().y;
	const unsigned offsetX = upscalingX
		? (width - map.getMapDimensions().x) / 2u
		: 0;
	const unsigned offsetY = upscalingY
		? (height - map.getMapDimensions().y) / 2u
		: 0;

	const sf::Vector2u end(
		std::min(width, map.getMapDimensions().x),
		std::min(height, map.getMapDimensions().y));

	copySourceRectToTarget(
		{ 0u, 0u },
		end,
		sf::Vector2i(offsetX, offsetY),
		tileValues,
		solidValues,
		width);

	map.build(tileValues, solidValues, { width, height });
}

void ToolMesh::shrinkTo(TileRect const& boundingBox)
{
	const auto width = boundingBox.right - boundingBox.left + 1;
	const auto height = boundingBox.bottom - boundingBox.top + 1;

	auto tileValues = std::vector<int>(width * height, 0);
	auto solidValues = std::vector<int>(width * height, 0);

	const int offsetX = -boundingBox.left;
	const int offsetY = -boundingBox.top;

	const sf::Vector2i translation(
		boundingBox.left,
		boundingBox.top);

	copySourceRectToTarget(
		{ boundingBox.left, boundingBox.top },
		{ boundingBox.right + 1, boundingBox.bottom + 1 },
		-translation,
		tileValues,
		solidValues,
		width);

	map.build(tileValues, solidValues, { width, height });
}

void ToolMesh::saveTo(LevelD& lvd) const
{
	lvd.mesh.tileWidth = sidebarUser.getClip().getFrameSize().x;
	lvd.mesh.tileHeight = sidebarUser.getClip().getFrameSize().y;
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

	if (dragging && (mode == DrawMode::RectEdge || mode == DrawMode::RectFill))
	{
		canvas->draw(rectShape);
	}
}

ExpectedPropertyPtr ToolMesh::getProperty(const sf::Vector2i& penPos) const
{
	auto tilePos = worldToTilePos(penPos);
	if (not isPositionValid(tilePos))
		return std::unexpected(BaseError());

	auto&& result = Box<MeshToolProperty>();

	result->tileX = tilePos.x;
	result->tileY = tilePos.y;
	result->tileValue = map.getTileValue(tilePos);
	result->blocking = map.isTileSolid(tilePos);
	result->defaultBlocking = defaultBlocks[result->tileValue];
	//result->imageTexture = sidebarUser.getSpriteAsTexture(result->tileValue);

	return std::move(result);
}

void ToolMesh::setProperty(const ToolPropertyInterface& prop)
{
	auto&& property = dynamic_cast<const MeshToolProperty&>(prop);

	commandQueue->push<SetTilePropertyCommand>(
		map,
		sf::Vector2u(property.tileX, property.tileY),
		property.tileValue,
		property.blocking);
}

void ToolMesh::buildCtxMenuInternal(tgui::MenuBar::Ptr& menu)
{
	const std::string OPTION_PENCIL = "Pencil Mode (Shift+P)";
	const std::string OPTION_FILL = "Rect-fill Mode (Shift+F)";
	const std::string OPTION_EDGE = "Rect-edge Mode (Shift+E)";
	const std::string OPTION_OVERLAY = "Toggle overlay (Shift+O)";

	addCtxMenuItem(menu, OPTION_PENCIL, [this] () { changeDrawingMode(DrawMode::Pencil); }, sf::Keyboard::P);
	addCtxMenuItem(menu, OPTION_FILL, [this] () { changeDrawingMode(DrawMode::RectFill); }, sf::Keyboard::F);
	addCtxMenuItem(menu, OPTION_EDGE, [this] () { changeDrawingMode(DrawMode::RectEdge); }, sf::Keyboard::E);
	addCtxMenuItem(menu, OPTION_OVERLAY, [this] () { enableOverlay = !enableOverlay; }, sf::Keyboard::O);
}

std::optional<TileRect> ToolMesh::getBoundingBox() const noexcept
{
	bool nonZeroTileDetected = false;
	TileRect result;

	for (unsigned y = 0; y < map.getMapDimensions().y; y++)
	{
		for (unsigned x = 0; x < map.getMapDimensions().x; x++)
		{
			if (map.getTileValue({ x, y }) == 0)
				continue;

			[[likely]]
			if (nonZeroTileDetected)
			{
				result.left = std::min(result.left, x);
				result.top = std::min(result.top, y);
				result.right = std::max(result.right, x);
				result.bottom = std::max(result.bottom, y);
			}
			else
			{
				nonZeroTileDetected = true;
				result = {
					.left = x,
					.top = y,
					.right = x,
					.bottom = y
				};
			}
		}
	}

	if (nonZeroTileDetected)
		return result;
	return {};
}

void ToolMesh::configure(
	std::filesystem::path const& texturePath,
	sf::Vector2u const& frameSize,
	sf::Vector2u const& frameSpacing,
	sf::IntRect const& textureBounds,
	std::vector<bool> const& defaultBlockSetting)
{
	defaultBlocks = defaultBlockSetting;

	sidebarUser.configure(
		texturePath,
		dgm::Clip(frameSize, textureBounds, 0, frameSpacing));
	map = DrawableLeveldMesh(
		sidebarUser.getTexture(),
		sidebarUser.getClip());

	rectShape.setOutlineColor(sf::Color(255, 0, 0, 128));
	rectShape.setOutlineThickness(2.f);
}

void ToolMesh::changeDrawingMode(ToolMesh::DrawMode newMode)
{
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

void MeshToolProperty::fillEditDialog(
		tgui::Panel::Ptr& panel,
		FormValidatorToken& formValidatorToken)
{
	using namespace DialogBuilderHelper;

	auto dst = tgui::ScrollablePanel::create();
	panel->add(dst);

	constexpr bool DISABLED = false; // the function accepts predicate enabled

	addOption(dst, formValidatorToken, "Tile X:", "X coordinate of the tile", tileX, 0, DISABLED);
	addOption(dst, formValidatorToken, "Tile Y:", "Y coordinate of the tile", tileY, 1, DISABLED);
	addOption(dst, "Impassable:", "Whether this tile blocks the player", blocking, 2);
	addOption(dst, "Impassable by default:", "Whether this type of tile is impassable by default", defaultBlocking, 3, DISABLED);
}
