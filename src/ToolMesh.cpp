#include "ToolMesh.hpp"
#include "JsonHelper.hpp"
#include "LogConsole.hpp"

unsigned Tilemap::getTile(unsigned tileX, unsigned tileY) {
	unsigned tileI = tileY * dataSize.x + tileX;
	sf::Vertex *quad = &vertices[tileI * size_t(4)];

	for (unsigned i = 0; i < clip.getFrameCount(); i++) {
		if (quad[0].texCoords.x == clip.getFrame(i).left && quad[0].texCoords.y == clip.getFrame(i).top) return i;
	}

	Log::write("Value of tile on coords[" + std::to_string(tileX) + ", " + std::to_string(tileY) + "] not found");
	return 0;
}

void Tilemap::drawArea(const sf::Vector2i& start, const sf::Vector2i& end, bool fill, unsigned tileValue, bool blocking) {
	unsigned startX = start.x / mesh.getVoxelSize().x;
	unsigned startY = start.y / mesh.getVoxelSize().y;
	unsigned endX = end.x / mesh.getVoxelSize().x;
	unsigned endY = end.y / mesh.getVoxelSize().y;

	Log::write("drawArea [" + std::to_string(startX) + ", " + std::to_string(startY) + "] -> [" +
		std::to_string(endX) + ", " + std::to_string(endY) + "]");

	for (unsigned y = startY; y <= endY; y++) {
		for (unsigned x = startX; x <= endX; x++) {
			if (!fill && !(x == startX || x == endX || y == startY || y == endY)) continue;

			changeTile(x, y, tileValue);
			mesh.at(x, y) = blocking;
		}
	}
}

void ToolMesh::configure(nlohmann::json &config) {
	std::string texturePath = config["layerTile"]["texture"]["path"];
	if (!texture.loadFromFile(texturePath)) {
		throw dgm::ResourceException("Cannot load texture file: " + texturePath);
	}

	sf::Vector2u tileDims = JsonHelper::arrayToVector2u(config["layerTile"]["texture"]["tileDimensions"]);
	sf::Vector2u tileOffs = JsonHelper::arrayToVector2u(config["layerTile"]["texture"]["tileOffsets"]);
	sf::IntRect  bounds = JsonHelper::arrayToIntRect(config["layerTile"]["texture"]["boundaries"]);

	defaultBlocks.resize(config["layerTile"]["defaultProperties"]["count"]);
	unsigned i = 0;
	for (auto &item : config["layerTile"]["defaultProperties"]["solids"]) {
		defaultBlocks[i++] = bool(int(item));
		Log::write(std::to_string(i) + ": " + std::to_string(bool(int(item))));
	}

	clip = dgm::Clip(tileDims, bounds, 0, tileOffs);
	tilemap.init(texture, clip);
	tilemap.mesh.setVoxelSize(clip.getFrameSize());

	rectShape.setOutlineColor(sf::Color(255, 0, 0, 128));
	rectShape.setOutlineThickness(2.f);
}

void ToolMesh::resize(unsigned width, unsigned height) {
	bgr.build({ width, height }, clip.getFrameSize());
	tilemap.build(clip.getFrameSize(), std::vector<int>(width * height, 0), { width, height });
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
}

void ToolMesh::drawTo(tgui::Canvas::Ptr &canvas) {
	canvas->draw(tilemap);
	bgr.draw(canvas);

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

	unsigned tileX = position.x / tilemap.mesh.getVoxelSize().x;
	unsigned tileY = position.y / tilemap.mesh.getVoxelSize().y;

	if (tileX < 0 || tileY < 0 || tileX >= tilemap.mesh.getDataSize().x || tileY >= tilemap.mesh.getDataSize().y) return;

	Log::write("Changing tile[" + std::to_string(tileX) + ", " + std::to_string(tileY) + "]");
	
	tilemap.drawArea(position, position, true, penTileId, defaultBlocks[penTileId]);
}

void ToolMesh::penUp() {
	if (!drawing) return;

	Log::write("penUp");
	drawing = false;

	if (mode == DrawMode::RectEdge) {
		tilemap.drawArea(penDownPos, penPos, false, penTileId, defaultBlocks[penTileId]);
	} else if (mode == DrawMode::RectFill) {
		tilemap.drawArea(penDownPos, penPos, true, penTileId, defaultBlocks[penTileId]);
	}
}

ToolProperty *ToolMesh::getProperty() {
	return nullptr;
}

void ToolMesh::setProperty(const ToolProperty &prop) {
	std::cerr << "ToolMesh::setProperty: Function is called, but is not implemented." << std::endl;
}

void ToolMesh::buildCtxMenu(tgui::MenuBar::Ptr &menu) {
	menu->removeMenu(CTX_MENU_NAME);
	menu->addMenu(CTX_MENU_NAME);

	const std::string OPTION_PENCIL = "Pencil Mode (Shift+P)";
	const std::string OPTION_FILL = "Rect-fill Mode (Shift+F)";
	const std::string OPTION_EDGE = "Rect-edge Mode (Shift+E)";

	menu->addMenuItem(OPTION_PENCIL);
	menu->connectMenuItem(CTX_MENU_NAME, OPTION_PENCIL, [this]() { changeDrawingMode(DrawMode::Pencil); });
	menu->addMenuItem(OPTION_FILL);
	menu->connectMenuItem(CTX_MENU_NAME, OPTION_FILL, [this]() { changeDrawingMode(DrawMode::RectFill); });
	menu->addMenuItem(OPTION_EDGE);
	menu->connectMenuItem(CTX_MENU_NAME, OPTION_EDGE, [this]() { changeDrawingMode(DrawMode::RectEdge); });

	clearShortcuts();
	registerShortcut(sf::Keyboard::P, [this]() { changeDrawingMode(DrawMode::Pencil); });
	registerShortcut(sf::Keyboard::F, [this]() { changeDrawingMode(DrawMode::RectFill); });
	registerShortcut(sf::Keyboard::E, [this]() { changeDrawingMode(DrawMode::RectEdge); });
}

void ToolMesh::destroyCtxMenu(tgui::MenuBar::Ptr& menu) {
	menu->removeMenu("Tool Menu");
}

void ToolMesh::buildSidebar(tgui::Gui &gui, tgui::Group::Ptr &sidebar, tgui::Theme &theme) {
	const float SIDEBAR_WIDTH = sidebar->getSize().x;

	// + button
	auto addbtn = tgui::Button::create("+");
	addbtn->setRenderer(theme.getRenderer("Button"));
	addbtn->setSize(SIDEBAR_WIDTH - 20.f, SIDEBAR_WIDTH - 20.f);
	addbtn->setPosition(10.f, 10.f);
	addbtn->connect("clicked", [this, &gui] () {
		buildTileIdSelectionModal(gui);
	});
	sidebar->add(addbtn);

	// ### History buttons
/*	BrushHistory& history = editor.getActiveHistory();
	ToolRenderer& brush = editor.getActiveBrush();

	const float HISTORY_BUTTONS_HEIGHT = app->window.getSize().y - TOPBAR_HEIGHT - SIDEBAR_WIDTH;
	const unsigned BUTTON_COUNT = unsigned(HISTORY_BUTTONS_HEIGHT / SIDEBAR_WIDTH);
	history.setLimit(BUTTON_COUNT);

	for (unsigned i = 0; i < history.getSize(); i++) {
		auto btn = tgui::Button::create();
		btn->getRenderer()->setTexture(brush.getTguiTextureForItem(history[i]));
		btn->setSize(SIDEBAR_WIDTH - 20.f, SIDEBAR_WIDTH - 20.f);
		btn->setPosition(10.f, SIDEBAR_WIDTH + i * (SIDEBAR_WIDTH - 10.f));

		if (history.getActive() != i) {
			btn->getRenderer()->setOpacity(0.2f);
		}

		btn->connect("pressed", [this, &history, i]() {
			if (history.getActive() == i) return;

			history.setActive(i);
			buildSidebar();
		});

		sidebar->add(btn);
	}*/
}

void ToolMesh::buildTileIdSelectionModal(tgui::Gui &gui) {
	const float SCROLLBAR_WIDTH = 20.f;

	// Create wrapper window
	auto modal = tgui::ChildWindow::create("Tile Selection");
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
	for (unsigned i = 0; i < clip.getFrameCount(); i++) {
		auto btn = tgui::Button::create();
		btn->getRenderer()->setTexture(tgui::Texture(texture, clip.getFrame(i)));
		btn->setSize(BUTTON_SIZE_OUTER - BUTTON_MARGIN, BUTTON_SIZE_OUTER - BUTTON_MARGIN);
		btn->setPosition(x * BUTTON_SIZE_OUTER + BUTTON_MARGIN, y * BUTTON_SIZE_OUTER + BUTTON_MARGIN);

		// User chosen a particular tile
		btn->connect("pressed", [this, i, &gui] () {
			auto modal = gui.get<tgui::ChildWindow>("ToolSelection");
			modal->close();
			penTileId = i;
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