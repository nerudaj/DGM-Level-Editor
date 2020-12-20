#include "Editor.hpp"
#include <fstream>
#include "JsonHelper.hpp"

const sf::Vector2f LEFT_VEC(-24.f, 0.f);
const sf::Vector2f UP_VEC(0.f, -24.f);
const sf::Vector2f DOWN_VEC(0.f, 24.f);
const sf::Vector2f RIGHT_VEC(24.f, 0.f);

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
		if (event.key.code == sf::Keyboard::Left) camera.move(LEFT_VEC);
		else if (event.key.code == sf::Keyboard::Up) camera.move(UP_VEC);
		else if (event.key.code == sf::Keyboard::Down) camera.move(DOWN_VEC);
		else if (event.key.code == sf::Keyboard::Right) camera.move(RIGHT_VEC);
	}
	else if (event.type == sf::Event::MouseWheelScrolled && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
		camera.zoom(event.mouseWheelScroll.delta * -0.25f);
		std::cout << "Zoom: " << camera.getCurrentZoomLevel() << std::endl;
	}
}

void Editor::draw() {
	if (!initialized) return;

	stateMgr.getTool().drawTo(canvas);
	canvas->draw(mouseIndicator);
}

void Editor::init(unsigned levelWidth, unsigned levelHeight, const std::string& configPath) {
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
	canvas->connect("RightMousePressed", [this] () { /* TODO: enable drawing */ });
	canvas->connect("RightMouseReleased", [this] () { /* TODO: disable drawing */ });
	canvas->connect("MousePressed", [this] () { stateMgr.getTool().penDown(); });
	canvas->connect("MouseReleased", [this] () { stateMgr.getTool().penUp(); });

	initialized = true;
	Editor::configPath = configPath; // Remember this and export to leveld later

	// By default selecting mesh tool
	switchTool("mesh");
}

void Editor::switchTool(const std::string &tool) {
	stateMgr.changeState(tool);
	stateMgr.getTool().buildSidebar(gui, theme);
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

Editor::Editor(tgui::Gui &gui, tgui::Theme &theme, tgui::Canvas::Ptr& canvas) : gui(gui), theme(theme), canvas(canvas) {
	// Instantiate all EditorTools here
	stateMgr.addState("mesh", new ToolMesh());

	// Bootstrapping mouse indicator
	mouseIndicator.setRadius(8.f);
	mouseIndicator.setFillColor(sf::Color::Green);
}
