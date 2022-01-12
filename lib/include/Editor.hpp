#pragma once

#include <DGM/dgm.hpp>
#include <TGUI/TGUI.hpp>
#include "Camera.hpp"
#include "EditorState.hpp"
#include "ToolMesh.hpp"
#include "ToolItem.hpp"
#include "ToolTrigger.hpp"
#include "NewLevelDialog.hpp"

class Editor {
private:
	tgui::Gui &gui;
	tgui::Theme &theme;
	tgui::Canvas::Ptr& canvas;
	ResizeDialog dialog = ResizeDialog(gui);
	Camera camera = Camera(canvas);
	EditorStateManager stateMgr;
	sf::CircleShape mouseIndicator;

	bool initialized = false;
	std::string configPath;
	sf::Vector2u levelSize;

	bool isMouseWithinBoundaries(const sf::Vector2f &mousePos) const;

	bool canScroll() const {
		// If property window is opened, prevent scrolling
		return gui.get<tgui::ChildWindow>("ToolPropertyModal") == nullptr;
	}

	bool canOpenPropertyDialog() const {
		// If property window is opened, do not open new one
		return canScroll();
	}

public:
	enum class ToolType : std::size_t {
		Mesh, Item, Trigger
	};

	bool isInitialized() const {
		return initialized;
	}

	void handleEvent(const sf::Event& event, const sf::Vector2i &mousePos);

	void draw();

	/**
	 *  Initialize Editor object with new level - it has some fixed width and height
	 *  Also there is path to config json which should be loaded and given to each
	 *  instantiated Tool.
	 */
	void init(unsigned levelWidth, unsigned levelHeight, const std::string& configPath);

	void switchTool(const ToolType tool);

	void loadFromFile(const std::string &filename);

	void saveToFile(const std::string &filename);

	void resizeDialog();

	Editor(tgui::Gui &gui, tgui::Theme &theme, tgui::Canvas::Ptr& canvas, std::function<void(void)> onStateChanged);
};