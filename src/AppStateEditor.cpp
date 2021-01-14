#include <cmath>

#include "AppStateEditor.hpp"
#include "FileApi.hpp"
#include "NewLevelDialog.hpp"
#include <iostream>

const float TOPBAR_HEIGHT = 22.f;
const float SIDEBAR_WIDTH = 100.f;

void AppStateEditor::input() {
	const sf::Vector2i mousePos = sf::Mouse::getPosition(app->window.getWindowContext());
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
		} else if (event.type == sf::Event::KeyPressed) {
			if (event.key.code == sf::Keyboard::LControl) {
				keyShortcut |= 1;
			}
			else if (event.key.code == sf::Keyboard::LShift) {
				keyShortcut |= 2;
			}
			else if (event.key.code == sf::Keyboard::S) {
				if (keyShortcut == 1) saveLevel(); // save
				else if (keyShortcut == 3) saveLevel(true); // save as
			}
			else if (event.key.code == sf::Keyboard::O && keyShortcut == 1) {
				loadLevel();
			}
			else if (event.key.code == sf::Keyboard::N && keyShortcut == 1) {
				dialogNewLevel.open([this] () { newLevelDialogCallback(); });
			}
		} else if (event.type == sf::Event::KeyReleased) {
			if (event.key.code == sf::Keyboard::LControl) {
				keyShortcut ^= 1;
			} else if (event.key.code == sf::Keyboard::LShift) {
				keyShortcut ^= 2;
			}
		}
		
		gui.handleEvent(event);
		editor.handleEvent(event, mousePos);
	}
}

void AppStateEditor::update() {}

void AppStateEditor::draw() {
	// Drawing canvas
	canvas->clear();

	editor.draw();

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
		std::cerr << "error:AppStateMainMenu: " << e.what() << std::endl;
	}

	// Setup resources
	sf::Font& font = resmgr.get<sf::Font>("cruft.ttf");

	// Gui setup
	theme.load(rootDir + "/resources/TransparentGrey.txt");
	
	gui.setFont(font);
	gui.setTarget(app->window.getWindowContext());

	buildLayout();

	setWindowTitle();

	return true;
}

const std::string FILE_CTX_NEW = "New (Ctrl+N)";
const std::string FILE_CTX_LOAD = "Load (Ctrl+O)";
const std::string FILE_CTX_SAVE = "Save (Ctrl+S)";
const std::string FILE_CTX_SAVE_AS = "Save (Ctrl+Shift+S)";
const std::string FILE_CTX_EXIT = "Exit";

void AppStateEditor::buildLayout() {
	// Canvas
	canvas = tgui::Canvas::create();
	// NOTE: If canvas is getting some "shadow" tile edits out of the view (under sidebar)
	// it might be because sidebar doesn't stop events from propagating downwards
	canvas->setSize(app->window.getSize().x, app->window.getSize().y - TOPBAR_HEIGHT);
	canvas->setPosition(0.f, TOPBAR_HEIGHT);
	gui.add(canvas, "TilesetCanvas");

	// Top bar
	auto menu = tgui::MenuBar::create();
	menu->setRenderer(theme.getRenderer("MenuBar"));
	menu->getRenderer()->setTextColor(sf::Color::Black);
	menu->setSize("100%", 22.f);
	menu->addMenu("File");
	menu->addMenuItem(FILE_CTX_NEW);
	menu->connectMenuItem("File", FILE_CTX_NEW, [this]() {
		dialogNewLevel.open([this]() { newLevelDialogCallback(); });
	});
	menu->addMenuItem(FILE_CTX_LOAD);
	menu->connectMenuItem("File", FILE_CTX_LOAD, [this]() { loadLevel(); });
	menu->addMenuItem(FILE_CTX_SAVE);
	menu->connectMenuItem("File", FILE_CTX_SAVE, [this]() { saveLevel(); });
	menu->addMenuItem(FILE_CTX_SAVE_AS);
	menu->connectMenuItem("File", FILE_CTX_SAVE_AS, [this] () { saveLevel(true); });
	menu->addMenuItem(FILE_CTX_EXIT);
	menu->connectMenuItem("File", FILE_CTX_EXIT, [this]() { app->popState(); });

	menu->addMenu("View");
	menu->addMenuItem("Console");
	menu->connectMenuItem("View", "Console", [this]() { Log::get().toggle(); });
	
	menu->addMenu("Editor");
	//menu->addMenuItem("Resize level");
	menu->addMenuItem("Tiles mode (T)");
	menu->connectMenuItem("Editor", "Tiles mode (T)", [this]() { editor.switchTool("mesh"); });
	menu->addMenuItem("Items mode (I)");
	menu->connectMenuItem("Editor", "Items mode (I)", [this]() { editor.switchTool("item"); });
	//menu->addMenuItem("Properties mode (P)");
	//menu->connectMenuItem("Editor", "Properties mode (P)", [this]() { switchEditorMode(EditorMode::Properties); });
	gui.add(menu, "TopMenuBar");

	// Side bar - only bootstrap the space it will be sitting in
	auto sidebar = tgui::Group::create();
	sidebar->setSize(SIDEBAR_WIDTH, app->window.getSize().y - TOPBAR_HEIGHT);
	sidebar->setPosition(app->window.getSize().x - SIDEBAR_WIDTH, TOPBAR_HEIGHT);
	gui.add(sidebar, "Sidebar");

	// Logger console
	Log::get().init(&gui);
	Log::get().create(theme, { 0.f, app->window.getSize().y - 200.f });
}

void AppStateEditor::buildPropertiesModal(unsigned x, unsigned y, unsigned id, uint16_t flags) {
	/*auto modal = tgui::ChildWindow::create("Set properties");
	modal->setRenderer(theme.getRenderer("ChildWindow"));
	modal->setSize(300, 400);
	modal->setPosition("35%", "35%");
	gui.add(modal, "ModalSetProperties");

	// Item image
	auto panel = tgui::Button::create();
	panel->setSize(120, 120);
	panel->setPosition(90, 10);
	panel->getRenderer()->setTexture(editor.getActiveBrush().getTguiTextureForItem(id));
	modal->add(panel);

	// Label
	auto label = tgui::Label::create("Set flags:");
	label->setSize(280, 20);
	label->setPosition(10, 140);
	modal->add(label);

	// EditBox
	auto edit = tgui::EditBox::create();
	edit->setSize(280, 20);
	edit->setPosition(10, 170);
	edit->setText(std::to_string(flags));
	modal->add(edit, "InputFlags");

	// Buttons
	auto btn = tgui::Button::create("Ok");
	btn->setRenderer(theme.getRenderer("Button"));
	btn->setSize("46%", "8%");
	btn->setPosition("2%", "90%");
	btn->connect("clicked", [x, y, this]() {
		EditorLayerItem &layer = dynamic_cast<EditorLayerItem&>(editor.getActiveLayer());
		uint8_t flags = std::stoul(gui.get<tgui::EditBox>("InputFlags")->getText().toAnsiString());
		layer.changeTileProperty(x, y, flags);
		gui.get<tgui::ChildWindow>("ModalSetProperties")->close();
	});
	modal->add(btn);

	btn = tgui::Button::create("Cancel");
	btn->setRenderer(theme.getRenderer("Button"));
	btn->setSize("46%", "8%");
	btn->setPosition("52%", "90%");
	btn->connect("clicked", [this]() {
		gui.get<tgui::ChildWindow>("ModalSetProperties")->close();
	});
	modal->add(btn);*/
}

void AppStateEditor::newLevelDialogCallback() {
	savePath = "";
	setWindowTitle();

	// Get settings from modal
	unsigned levelWidth = dialogNewLevel.getLevelWidth();
	unsigned levelHeight = dialogNewLevel.getLevelHeight();
	std::string configPath = dialogNewLevel.getConfigPath();

	Log::write("editor.init(" + std::to_string(levelWidth) + ", " + std::to_string(levelHeight) + ", " + configPath + ");");
	editor.init(levelWidth, levelHeight, configPath);
	ini["Editor"]["configPath"] = configPath;
}

void AppStateEditor::loadLevel() {
	Log::write("AppStateEditor::loadLevel(). savePath = " + savePath + "; empty = " + std::to_string(savePath.empty()));

	try {
		savePath = FileApi::getOpenFileName("LevelD Files\0*.lvd\0Any File\0*.*\0");
	} catch (...) { return; } // User cancel

	// The load path becomes save path for subsequent saves

	try {
		editor.loadFromFile(savePath);
		setWindowTitle(savePath);
	}
	catch (std::exception &e) {
		Log::write(e.what());
	}
}

void AppStateEditor::saveLevel(bool forceNewPath) {
	if (savePath.empty() || forceNewPath) {
		try {
			savePath = FileApi::getSaveFileName("LevelD Files\0*.lvd\0Any File\0*.*\0");
		} catch (...) { return; } // User cancel
	}

	try {
		editor.saveToFile(savePath);
		setWindowTitle(savePath);
	}
	catch (std::exception & e) {
		Log::write(e.what());
	}
}
