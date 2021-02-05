#include "ToolMesh.hpp"
#include "JsonHelper.hpp"
#include "LogConsole.hpp"

void ToolMesh::configure(nlohmann::json &config) {
	const std::string TOOL_STR = "toolMesh";

	std::string texturePath = config[TOOL_STR]["texture"]["path"];
	if (!texture.loadFromFile(texturePath)) {
		throw dgm::ResourceException("Cannot load texture file: " + texturePath);
	}

	sf::Vector2u tileDims = JsonHelper::arrayToVector2u(config[TOOL_STR]["texture"]["tileDimensions"]);
	sf::Vector2u tileOffs = JsonHelper::arrayToVector2u(config[TOOL_STR]["texture"]["tileOffsets"]);
	sf::IntRect  bounds = JsonHelper::arrayToIntRect(config[TOOL_STR]["texture"]["boundaries"]);

	defaultBlocks.resize(config[TOOL_STR]["defaultProperties"]["count"]);
	unsigned i = 0;
	for (auto &item : config[TOOL_STR]["defaultProperties"]["solids"]) {
		defaultBlocks[i++] = bool(int(item));
		Log::write(std::to_string(i) + ": " + std::to_string(bool(int(item))));
	}

	clip = dgm::Clip(tileDims, bounds, 0, tileOffs);
	tilemap.init(texture, clip);
	tilemap.mesh.setVoxelSize(clip.getFrameSize());
	tilemap.overlay.init();

	rectShape.setOutlineColor(sf::Color(255, 0, 0, 128));
	rectShape.setOutlineThickness(2.f);

	penHistory.clear();
}

void ToolMesh::resize(unsigned width, unsigned height) {
	bgr.build({ width, height }, clip.getFrameSize());
	tilemap.build(clip.getFrameSize(), std::vector<int>(width * height, 0), { width, height });
	tilemap.overlay.map.build(clip.getFrameSize(), std::vector<int>(width * height, 0), { width, height });
	tilemap.mesh.setDataSize(width, height);
}

void ToolMesh::saveTo(LevelD &lvd) {
	Log::write("ToolMesh::saveTo");
	lvd.mesh.tileWidth = clip.getFrameSize().x;
	lvd.mesh.tileHeight = clip.getFrameSize().y;
	lvd.mesh.layerWidth = tilemap.mesh.getDataSize().x;
	lvd.mesh.layerHeight = tilemap.mesh.getDataSize().y;

	lvd.mesh.layers.resize(1);
	lvd.mesh.layers[0].tiles.resize(lvd.mesh.layerWidth * lvd.mesh.layerHeight);
	lvd.mesh.layers[0].blocks.resize(lvd.mesh.layerWidth * lvd.mesh.layerHeight);
	for (unsigned tileY = 0, i = 0; tileY < lvd.mesh.layerHeight; tileY++) {
		for (unsigned tileX = 0; tileX < lvd.mesh.layerWidth; tileX++, i++) {
			lvd.mesh.layers[0].tiles[i] = tilemap.getTile(tileX, tileY);
			lvd.mesh.layers[0].blocks[i] = tilemap.mesh[i];
		}
	}
}

void ToolMesh::loadFrom(const LevelD &lvd) {
	Log::write("ToolMesh::loadFrom");
	resize(lvd.mesh.layerWidth, lvd.mesh.layerHeight);

	tilemap.build(lvd.mesh, 0);
	tilemap.mesh = dgm::Mesh(lvd.mesh, 0);
	tilemap.build(lvd.mesh, 0);
}

void ToolMesh::drawTo(tgui::Canvas::Ptr &canvas, uint8_t) {
	canvas->draw(tilemap);
	bgr.draw(canvas);

	if (enableOverlay) canvas->draw(tilemap.overlay.map);

	if (drawing) {
		if (mode == DrawMode::RectEdge || mode == DrawMode::RectFill) {
			canvas->draw(rectShape);
		}
	}
}

void ToolMesh::penDown() {
	Log::write("penDown");

	drawing = true;
	penDownPos = penPos;
	rectShape.setPosition(sf::Vector2f(penDownPos));

	if (mode == DrawMode::RectEdge) {
		rectShape.setFillColor(sf::Color::Transparent);
	}
	else if (mode == DrawMode::RectFill) {
		rectShape.setFillColor(sf::Color(128, 0, 0, 128));
	}
}

void ToolMesh::penPosition(const sf::Vector2i &position) {
	penPos = position;

	if (mode != DrawMode::Pencil) {
		rectShape.setSize(sf::Vector2f(position - penDownPos));
	}

	if (!(drawing && mode == DrawMode::Pencil)) return;
	Log::write("penPosition && drawing");

	auto tilePos = worldToTilePos(position);
	if (isPositionValid(tilePos)) {
		tilemap.drawArea(position, position, true, penValue, defaultBlocks[penValue]);
	}
}

void ToolMesh::penUp() {
	if (!drawing) return;

	Log::write("penUp");
	drawing = false;

	if (mode == DrawMode::RectEdge) {
		tilemap.drawArea(penDownPos, penPos, false, penValue, defaultBlocks[penValue]);
	} else if (mode == DrawMode::RectFill) {
		tilemap.drawArea(penDownPos, penPos, true, penValue, defaultBlocks[penValue]);
	}
}

ToolProperty &ToolMesh::getProperty() {
	tileProperty.clear();

	auto tilePos = worldToTilePos(penPos);
	if (not isPositionValid(tilePos)) return tileProperty; // returning null property

	tileProperty.tileX = tilePos.x;
	tileProperty.tileY = tilePos.y;
	tileProperty.tileValue = tilemap.getTile(tilePos.x, tilePos.y);
	tileProperty.blocking = tilemap.getTileBlock(tilePos.x, tilePos.y);
	tileProperty.defaultBlocking = defaultBlocks[tileProperty.tileValue];
	tileProperty.imageTexture = getSpriteAsTexture(tileProperty.tileValue);
	tileProperty.empty = false;

	return tileProperty;
}

void ToolMesh::setProperty(const ToolProperty &) {
	tilemap.setTile(tileProperty.tileX, tileProperty.tileY, tileProperty.tileValue, tileProperty.blocking);
}

void ToolMesh::buildCtxMenu(tgui::MenuBar::Ptr &menu) {
	Tool::buildCtxMenu(menu); // bootstrap

	const std::string OPTION_PENCIL = "Pencil Mode (Shift+P)";
	const std::string OPTION_FILL = "Rect-fill Mode (Shift+F)";
	const std::string OPTION_EDGE = "Rect-edge Mode (Shift+E)";
	const std::string OPTION_OVERLAY = "Toggle overlay (Shift+O)";

	addCtxMenuItem(menu, OPTION_PENCIL, [this]() { changeDrawingMode(DrawMode::Pencil); }, sf::Keyboard::P);
	addCtxMenuItem(menu, OPTION_FILL, [this]() { changeDrawingMode(DrawMode::RectFill); }, sf::Keyboard::F);
	addCtxMenuItem(menu, OPTION_EDGE, [this]() { changeDrawingMode(DrawMode::RectEdge); }, sf::Keyboard::E);
	addCtxMenuItem(menu, OPTION_OVERLAY, [this]() { enableOverlay = !enableOverlay; }, sf::Keyboard::O);
}

void ToolMesh::changeDrawingMode(ToolMesh::DrawMode newMode) {
	Log::write("Mesh: Changing draw mode to " + std::to_string(newMode));
	mode = newMode;
}

std::string std::to_string(ToolMesh::DrawMode mode) {
	if (mode == ToolMesh::DrawMode::Pencil) return "Pencil";
	else if (mode == ToolMesh::DrawMode::RectFill) return "RectFill";
	else if (mode == ToolMesh::DrawMode::RectEdge) return "RectEdge";
	else if (mode == ToolMesh::DrawMode::Line) return "Line";
	return "Error";
}

void MeshToolProperty::buildModalSpecifics(tgui::ScrollablePanel::Ptr& dst) {
	constexpr bool DISABLED = false; // the function accepts predicate enabled

	addOption(dst, "Tile X:", "X coordinate of the tile", tileX, 0, DISABLED);
	addOption(dst, "Tile Y:", "Y coordinate of the tile", tileY, 1, DISABLED);
	addOption(dst, "Impassable:", "Whether this tile blocks the player", blocking, 2);
	addOption(dst, "Impassable by default:", "Whether this type of tile is impassable by default", defaultBlocking, 3, DISABLED);
}
