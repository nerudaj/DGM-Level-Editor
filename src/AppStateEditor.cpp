#include <cmath>

#include "AppStateEditor.hpp"
#include "AppStateIngame.hpp"
#include "FileApi.hpp"

const float TOPBAR_HEIGHT = 22.f;
const float SIDEBAR_WIDTH = 100.f;

void AppStateEditor::input() {
	sf::Event event;
	while (app->window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			app->window.close();
		}
		else if (event.type == sf::Event::KeyPressed) {
			if (event.key.code == sf::Keyboard::T) switchEditorMode(EditorMode::Tiles);
			else if (event.key.code == sf::Keyboard::I) switchEditorMode(EditorMode::Items);
		}
		else if (event.type == sf::Event::MouseWheelScrolled && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
			zoomLevel += event.mouseWheelScroll.delta * 0.5f;
			editor.setZoomLevel(zoomLevel);
		}
		else if (event.type == sf::Event::MouseButtonPressed) {
			lastMouseButtonPressed = event.mouseButton.button;
		}

		gui.handleEvent(event);
	}
}

void AppStateEditor::update() {
	if (drawing) {
		drawOnLayer();
	}
}

void AppStateEditor::draw() {
	// Drawing canvas
	canvas->clear();

	editor.draw(canvas);

	canvas->display();

	// Whole window
	app->window.beginDraw(sf::Color::White);

	gui.draw();

	app->window.endDraw();
}

bool AppStateEditor::init() {
	// Setup resources
	sf::Font& font = resmgr.get<sf::Font>("cruft.ttf");

	// Setup member variables
	levelWidth = 20;
	levelHeight = 10;
	zoomLevel = 1.f;
	drawing = false;
	lastMouseButtonPressed = sf::Mouse::Left;
	editor.init(levelWidth, levelHeight, resmgr);

	// Gui setup
	theme.load(rootDir + "/graphics/gui/TransparentGrey.txt");
	
	gui.setFont(font);
	gui.setTarget(app->window.getWindowContext());

	buildLayout();

	return true;
}

void AppStateEditor::buildLayout() {
	// Canvas
	canvas = tgui::Canvas::create();
	canvas->setSize(app->window.getSize().x - SIDEBAR_WIDTH, app->window.getSize().y - TOPBAR_HEIGHT);
	canvas->setPosition(0.f, TOPBAR_HEIGHT);
	canvas->connect("MousePressed", [this]() { 
		drawing = true; 
	});
	canvas->connect("MouseReleased", [this]() { drawing = false; });
	gui.add(canvas, "TilesetCanvas");

	// Top bar
	auto menu = tgui::MenuBar::create();
	menu->setRenderer(theme.getRenderer("MenuBar"));
	menu->getRenderer()->setTextColor(sf::Color::Black);
	menu->setSize("100%", 22.f);
	menu->addMenu("File");
	menu->addMenuItem("New");
	menu->addMenuItem("Load");
	menu->connectMenuItem("File", "Load", [this]() {
		try {
			editor.loadFromFile(FileApi::getOpenFileName());
		}
		catch (...) {}
	});
	menu->addMenuItem("Save");
	menu->connectMenuItem("File", "Save", [this]() {
		try {
			editor.saveToFile(FileApi::getSaveFileName());
		}
		catch (...) {}
	});
	menu->addMenuItem("Exit");
	menu->connectMenuItem("File", "Exit", [this]() { app->popState(); });

	menu->addMenu("View");
	menu->addMenuItem("Console");
	menu->connectMenuItem("View", "Console", [this]() {
		auto child = gui.get<tgui::ChildWindow>("ConsoleWindow");
		child->setVisible(!child->isVisible());
	});
	
	menu->addMenu("Editor");
	menu->addMenuItem("Tiles mode (T)");
	menu->connectMenuItem("Editor", "Tiles mode (T)", [this]() { switchEditorMode(EditorMode::Tiles); });
	menu->addMenuItem("Items mode (I)");
	menu->connectMenuItem("Editor", "Items mode (I)", [this]() { switchEditorMode(EditorMode::Items); });
	menu->addMenuItem("Play level");
	menu->connectMenuItem("Editor", "Play level", [this]() { playLevel(); });
	gui.add(menu);

	// Side bar
	auto sidebar = tgui::Group::create();
	sidebar->setSize(SIDEBAR_WIDTH, app->window.getSize().y - TOPBAR_HEIGHT);
	sidebar->setPosition(app->window.getSize().x - SIDEBAR_WIDTH, TOPBAR_HEIGHT);
	gui.add(sidebar, "Sidebar");

	buildSidebar();

	// Logger console
	auto child = tgui::ChildWindow::create();
	child->setRenderer(theme.getRenderer("ChildWindow"));
	child->setResizable(true);
	child->setSize(400.f, 200.f);
	child->setPosition(0.f, app->window.getSize().y - 200.f);
	child->setTitle("Console");
	child->setTitleButtons(tgui::ChildWindow::TitleButton::Close);
	child->connect("Closed", [this]() {
		auto child = gui.get<tgui::ChildWindow>("ConsoleWindow");
		child->setVisible(false);
	});
	gui.add(child, "ConsoleWindow");

	auto console = tgui::ChatBox::create();
	console->setRenderer(theme.getRenderer("ChatBox"));
	console->setSize("100%", "100%");
	console->setTextSize(14);
	console->setLinesStartFromTop();
	console->addLine("This is a log console");
	child->add(console, "LogConsole");
}

void AppStateEditor::buildSidebar() {
	auto sidebar = gui.get<tgui::Group>("Sidebar");
	sidebar->removeAllWidgets();

	// ## Components of side bar
	// ### Back panel
	auto sidebgr = tgui::Panel::create({ "100%", "100%" });
	sidebgr->setRenderer(theme.getRenderer("Panel"));
	sidebar->add(sidebgr);

	// ### + button
	auto addbtn = tgui::Button::create("+");
	addbtn->setRenderer(theme.getRenderer("Button"));
	addbtn->setSize(SIDEBAR_WIDTH - 20.f, SIDEBAR_WIDTH - 20.f);
	addbtn->setPosition(10.f, 10.f);
	addbtn->connect("clicked", [this]() { buildSelectionModal(); });
	sidebar->add(addbtn);

	// ### History buttons
	EditorHistory& history = editor.getActiveHistory();
	EditorBrush& brush = editor.getActiveTool();

	const float HISTORY_BUTTONS_HEIGHT = app->window.getSize().y - TOPBAR_HEIGHT - SIDEBAR_WIDTH;
	const unsigned BUTTON_COUNT = unsigned(HISTORY_BUTTONS_HEIGHT / SIDEBAR_WIDTH);
	const size_t LIMIT = std::min(size_t(BUTTON_COUNT), history.getSize());

	for (unsigned i = 0; i < LIMIT; i++) {
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
	}
}

void AppStateEditor::buildSelectionModal() {
	const float SCROLLBAR_WIDTH = 20.f;

	// Create wrapper window
	auto modal = tgui::ChildWindow::create("Tool Selection");
	modal->setSize("50%", "50%");
	modal->setPosition("25%", "25%");
	gui.add(modal, "ToolSelection");

	// Create scrollable group inside of this window
	auto group = tgui::ScrollablePanel::create();
	group->getRenderer()->setScrollbarWidth(SCROLLBAR_WIDTH);
	modal->add(group);

	// Compute button widths when there are 4 buttons per row
	const unsigned BUTTONS_PER_ROW = 4;
	const float BUTTON_SIZE_OUTER = (app->window.getSize().x * 0.5f - SCROLLBAR_WIDTH) / BUTTONS_PER_ROW;
	const float BUTTON_MARGIN = 10.f;

	// Render all buttons
	EditorHistory& history = editor.getActiveHistory();
	EditorBrush& brush = editor.getActiveTool();
	unsigned x = 0, y = 0;
	for (size_t i = 0; i < brush.getItemCount(); i++) {
		auto btn = tgui::Button::create();
		btn->getRenderer()->setTexture(brush.getTguiTextureForItem(i));
		btn->setSize(BUTTON_SIZE_OUTER - BUTTON_MARGIN, BUTTON_SIZE_OUTER - BUTTON_MARGIN);
		btn->setPosition(x * BUTTON_SIZE_OUTER + BUTTON_MARGIN, y * BUTTON_SIZE_OUTER + BUTTON_MARGIN);

		// When button is clicked, selected item will be added to history
		btn->connect("pressed", [this, &history, i]() {
			auto modal = gui.get<tgui::ChildWindow>("ToolSelection");
			modal->close();
			history.addItem(i);
			buildSidebar();
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

void AppStateEditor::switchEditorMode(EditorMode mode) {
	editor.setMode(mode);

	log("Switched to " + std::to_string(mode) + " mode");

	buildSidebar();
}

void AppStateEditor::drawOnLayer() {
	auto canvas = gui.get<tgui::Canvas>("TilesetCanvas");
	auto mousePos = sf::Vector2f(sf::Mouse::getPosition(app->window.getWindowContext()));
	auto relativeMousePos = mousePos - canvas->getPosition();

	auto relativeTileSize = sf::Vector2f(editor.getTileSize()) * zoomLevel;
	unsigned tileX = unsigned(relativeMousePos.x / relativeTileSize.x);
	unsigned tileY = unsigned(relativeMousePos.y / relativeTileSize.y);

	if (levelWidth > tileX && levelHeight > tileY) {
		EditorHistory& history = editor.getActiveHistory();
		unsigned value = lastMouseButtonPressed == sf::Mouse::Left ? history[history.getActive()] : 0;
		editor.getActiveLayer().changeTile(tileX, tileY, value);
	}
}

void AppStateEditor::playLevel() {
	// TODO: Bootstrap what you need

	app->pushState(new AppStateIngame(resmgr));
}

void AppStateEditor::log(const std::string& message) {
	auto logger = gui.get<tgui::ChatBox>("LogConsole");
	logger->addLine(message);
}
