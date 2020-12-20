#pragma once

#include <DGM/dgm.hpp>
#include <TGUI/TGUI.hpp>
#include "Camera.hpp"
#include "EditorState.hpp"
#include "ToolMesh.hpp"

class Editor {
private:
	tgui::Gui &gui;
	tgui::Theme &theme;
	tgui::Canvas::Ptr& canvas;
	Camera camera = Camera(canvas);
	EditorStateManager stateMgr;
	sf::CircleShape mouseIndicator;

	bool initialized = false;
	std::string configPath;

	bool isMouseWithinBoundaries(const sf::Vector2f &mousePos) const;

public:
	void handleEvent(const sf::Event& event, const sf::Vector2i &mousePos);

	void draw();

	/**
	 *  Initialize Editor object with new level - it has some fixed width and height
	 *  Also there is path to config json which should be loaded and given to each
	 *  instantiated Tool.
	 */
	void init(unsigned levelWidth, unsigned levelHeight, const std::string& configPath);

	void switchTool(const std::string &tool);

	/*void selectToolProperty(tgui::Gui& gui) {
		stateMgr.getTool().getTool().buildPropertySelectionModal(gui);
	}*/

	void loadFromFile(const std::string &filename);

	void saveToFile(const std::string &filename);

	Editor(tgui::Gui &gui, tgui::Theme &theme, tgui::Canvas::Ptr& canvas);
};