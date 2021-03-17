#include "Editor.hpp"
#include <fstream>
#include "JsonHelper.hpp"
#include "LogConsole.hpp"

const sf::Vector2f LEFT_VEC(-24.f, 0.f);
const sf::Vector2f UP_VEC(0.f, -24.f);
const sf::Vector2f DOWN_VEC(0.f, 24.f);
const sf::Vector2f RIGHT_VEC(24.f, 0.f);

namespace std {
	constexpr const char *to_string(const Editor::ToolType type) {
		if (type == Editor::ToolType::Mesh) return "mesh";
		else if (type == Editor::ToolType::Item) return "item";
		else if (type == Editor::ToolType::Trigger) return "trigger";
		return "error";
	}
}

bool Editor::isMouseWithinBoundaries(const sf::Vector2f& mousePos) const {
	return mousePos.x < 0.f && mousePos.y < 0.f;
}

void Editor::handleEvent(const sf::Event& event, const sf::Vector2i& mousePos) {
	if (!initialized) return;

	// Update mouse position for both indicator and current tool
	auto realMousePos = camera.getWorldCoordinates(sf::Vector2f(mousePos));
	mouseIndicator.setPosition(realMousePos);
	stateMgr.getTool().penPosition(sf::Vector2i(realMousePos));

	if (event.type == sf::Event::KeyPressed) {
		if (event.key.code == sf::Keyboard::Left && canScroll()) camera.move(LEFT_VEC);
		else if (event.key.code == sf::Keyboard::Up && canScroll()) camera.move(UP_VEC);
		else if (event.key.code == sf::Keyboard::Down && canScroll()) camera.move(DOWN_VEC);
		else if (event.key.code == sf::Keyboard::Right && canScroll()) camera.move(RIGHT_VEC);
		else if (event.key.code == sf::Keyboard::Escape) stateMgr.getTool().penCancel();
		else if (event.key.code == sf::Keyboard::Delete) stateMgr.getTool().penDelete();
	}
	else if (event.type == sf::Event::MouseWheelScrolled && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
		camera.zoom(event.mouseWheelScroll.delta * -0.25f);
		std::cout << "Zoom: " << camera.getCurrentZoomLevel() << std::endl;
	}

	stateMgr.getTool().handleShortcuts(event);
}

void Editor::draw() {
	if (!initialized) return;

	stateMgr.forallStates([this] (Tool& tool, bool active) {
		tool.drawTo(canvas, active ? 255 : 128);
	});
	canvas->draw(mouseIndicator);
}

void Editor::init(unsigned levelWidth, unsigned levelHeight, const std::string& configPath) {
	levelSize = { levelWidth, levelHeight };

	// Load json config and configure all tools
	auto config = JsonHelper::loadFromFile(configPath);
	stateMgr.forallStates([levelWidth, levelHeight, &config] (Tool &tool) {
		tool.configure(config);
		tool.resize(levelWidth, levelHeight);
	});

	// Configure camera
	camera.init();
	camera.resetPosition();
	camera.resetZoom();

	// Configure canvas callbacks
	canvas->connect("RightMousePressed", [this] () {
		Log::write("RMB down");
	});
	canvas->connect("RightMouseReleased", [this] () {
		Log::write("RMB up");
		if (not canOpenPropertyDialog()) return;
		auto& prop = stateMgr.getTool().getProperty();
		if (!prop.isEmpty()) prop.buildModal();
	});
	canvas->connect("MousePressed", [this] () { stateMgr.getTool().penDown(); });
	canvas->connect("MouseReleased", [this] () { stateMgr.getTool().penUp(); });

	initialized = true;
	Editor::configPath = configPath; // Remember this and export to leveld later

	// By default selecting mesh tool
	switchTool(ToolType::Mesh);
}

void Editor::switchTool(const ToolType tool) {
	stateMgr.changeState(std::to_string(tool));
	stateMgr.getTool().buildSidebar(theme);

	auto menu = gui.get<tgui::MenuBar>("TopMenuBar");
	stateMgr.getTool().buildCtxMenu(menu);
}

void Editor::loadFromFile(const std::string &filename) {
	LevelD lvd;
	lvd.loadFromFile(filename);

	init(1, 1, lvd.metadata.description); // Currently using this to be able to load the config
	stateMgr.forallStates([&lvd] (Tool &tool) {
		tool.loadFrom(lvd);
	});
}

void Editor::saveToFile(const std::string &filename) {
	LevelD lvd;
	lvd.metadata.description = configPath; // Currently using this to be able to load the config
	stateMgr.forallStates([&lvd] (Tool &tool) {
		tool.saveTo(lvd);
	});
	lvd.saveToFile(filename);
}

void Editor::resizeDialog() {
	dialog.open([this] () {
		unsigned width = dialog.getLevelWidth();
		unsigned height = dialog.getLevelHeight();
		stateMgr.forallStates([&] (Tool& tool) {
			tool.resize(width, height);
		});
	});
}

Editor::Editor(tgui::Gui &gui, tgui::Theme &theme, tgui::Canvas::Ptr& canvas) : gui(gui), theme(theme), canvas(canvas) {
	// Instantiate all EditorTools here
	stateMgr.addState(std::to_string(ToolType::Mesh), new ToolMesh(gui));
	stateMgr.addState(std::to_string(ToolType::Item), new ToolItem(gui));
	stateMgr.addState(std::to_string(ToolType::Trigger), new ToolTrigger(gui));

	// Bootstrapping mouse indicator
	mouseIndicator.setRadius(8.f);
	mouseIndicator.setFillColor(sf::Color::Green);
}
