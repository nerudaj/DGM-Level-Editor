#include <cmath>

#include "AppStateEditor.hpp"
#include "FileApi.hpp"
#include <fstream>

const float TOPBAR_HEIGHT = 22.f;
const float SIDEBAR_WIDTH = 100.f;

void AppStateEditor::input() {
	sf::Event event;
	while (app->window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			app->window.close();
		}
		else if (event.type == sf::Event::Resized) {
			app->window.getWindowContext().setView(sf::View(sf::FloatRect(0.f, 0.f, static_cast<float>(event.size.width), static_cast<float>(event.size.height))));
			gui.removeAllWidgets();
			gui.setView(app->window.getWindowContext().getView());
			buildLayout();
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
	try {
		resmgr.loadResource<sf::Font>(rootDir + "/resources/cruft.ttf");
	}
	catch (std::exception& e) {
		std::ofstream save("log.txt");
		save << "error:AppStateMainMenu: " << e.what() << std::endl;
		save.close();
		save.clear();
		return false;
	}

	// Setup resources
	sf::Font& font = resmgr.get<sf::Font>("cruft.ttf");

	// Setup member variables
	zoomLevel = 1.f;
	drawing = false;
	lastMouseButtonPressed = sf::Mouse::Left;

	// Gui setup
	theme.load(rootDir + "/resources/TransparentGrey.txt");
	
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
	menu->connectMenuItem("File", "New", [this]() {
		buildNewLevelModal();
	});
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
	menu->connectMenuItem("Editor", "Play level", [this]() { editor.playLevel(); });
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
	EditorBrush& brush = editor.getActiveBrush();

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
	EditorBrush& brush = editor.getActiveBrush();
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

void AppStateEditor::buildNewLevelModal() {
	auto modal = tgui::ChildWindow::create("New level");
	modal->setSize("30%", "50%");
	modal->setPosition("35%", "25%");
	gui.add(modal, "ModalNewLevel");

	const auto ROW_HEIGHT = "6%";
	const auto ROW_Y_OFFSET = 2;

	// Labels
	std::vector<std::string> labels = {
		"Level width:",
		"Level height:",
		"Config file:"
	};

	for (unsigned i = 0; i < labels.size(); i++) {
		auto label = tgui::Label::create(labels[i]);
		label->setSize("26%", ROW_HEIGHT);
		label->setPosition("2%", std::to_string(i * 10 + ROW_Y_OFFSET) + "%");
		label->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
		modal->add(label);
	}

	// Textboxes
	std::vector<std::vector<std::string>> boxes = {
		{"66%", "20", "InputLevelWidth"},
		{"66%", "10", "InputLevelHeight"},
		{"58%", rootDir + "/config.json", "InputLevelConfig"},
	};

	for (unsigned i = 0; i < boxes.size(); i++) {
		auto box = tgui::EditBox::create();
		box->setSize(boxes[i][0], ROW_HEIGHT);
		box->setPosition("32%", std::to_string(i * 10 + ROW_Y_OFFSET) + "%");
		box->setText(boxes[i][1]);
		modal->add(box, boxes[i][2]);
	}

	// ## Buttons
	// Selection of config file
	auto btn = tgui::Button::create("...");
	btn->setSize("8%", "6%");
	btn->setPosition("90%", "22%");
	btn->connect("clicked", [this]() {
		try {
			auto str = FileApi::getOpenFileName();
			auto box = gui.get<tgui::EditBox>("InputLevelConfig");
			box->setText(str);
		}
		catch (...) {}
	});
	modal->add(btn);

	btn = tgui::Button::create("Ok");
	btn->setSize("20%", "8%");
	btn->setPosition("56%", "90%");
	btn->connect("clicked", [this]() { newLevel(); });
	modal->add(btn);

	btn = tgui::Button::create("Cancel");
	btn->setSize("20%", "8%");
	btn->setPosition("78%", "90%");
	btn->connect("clicked", [this]() {
		auto modal = gui.get<tgui::ChildWindow>("ModalNewLevel");
		modal->close();
	});
	modal->add(btn);
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

void AppStateEditor::newLevel() {
	// Get settings from modal
	// TODO: error catching
	levelWidth = std::stoul(gui.get<tgui::EditBox>("InputLevelWidth")->getText().toAnsiString());
	levelHeight = std::stoul(gui.get<tgui::EditBox>("InputLevelHeight")->getText().toAnsiString());
	std::string configPath = gui.get<tgui::EditBox>("InputLevelConfig")->getText().toAnsiString();

	// Close modal
	auto modal = gui.get<tgui::ChildWindow>("ModalNewLevel");
	modal->close();

	editor.init(levelWidth, levelHeight, configPath);
	buildSidebar();
}

void AppStateEditor::log(const std::string& message) {
	auto logger = gui.get<tgui::ChatBox>("LogConsole");
	logger->addLine(message);
}
