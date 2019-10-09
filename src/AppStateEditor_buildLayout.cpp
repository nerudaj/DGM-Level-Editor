#include "AppStateEditor.hpp"

void AppStateEditor::buildLayout() {
	// Canvas
	canvas = tgui::Canvas::create();
	canvas->setSize(app->window.getSize().x - SIDEBAR_WIDTH, app->window.getSize().y - TOPBAR_HEIGHT);
	canvas->setPosition(0.f, TOPBAR_HEIGHT);
	canvas->connect("RightMousePressed", [this]() { drawing = true; });
	canvas->connect("RightMouseReleased", [this]() { drawing = false; });
	canvas->connect("MousePressed", [this]() { drawing = true; });
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
			editor.loadFromFile(FileApi::getOpenFileName("LevelD Files\0*.lvd\0Any File\0*.*\0"));
		}
		catch (...) {}
	});
	menu->addMenuItem("Save");
	menu->connectMenuItem("File", "Save", [this]() {
		try {
			editor.saveToFile(FileApi::getSaveFileName("LevelD Files\0*.lvd\0Any File\0*.*\0"));
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
	menu->addMenuItem("Properties mode (P)");
	menu->connectMenuItem("Editor", "Properties mode (P)", [this]() { switchEditorMode(EditorMode::Properties); });
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