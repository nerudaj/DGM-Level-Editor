#pragma once

#include <DGM/dgm.hpp>
#include <TGUI/TGUI.hpp>
#include "Camera.hpp"
#include "EditorState.hpp"
#include "Tools/ToolMesh.hpp"
#include "Tools/ToolItem.hpp"
#include "Tools/ToolTrigger.hpp"
#include "Dialogs/ResizeLevelDialog.hpp"

class EditorInterface
{
public:
	enum class ToolType : std::size_t
	{
		Mesh, Item, Trigger
	};

	virtual bool isInitialized() const = 0;

	virtual void draw() = 0;

	virtual void init(unsigned levelWidth, unsigned levelHeight, const std::string& configPath) = 0;

	virtual void handleEvent(const sf::Event& event, const sf::Vector2i& mousePos) = 0;

	virtual void saveToFile(const std::string& filename) = 0;

	virtual void loadFromFile(const std::string& filename) = 0;

	virtual void switchTool(const ToolType tool) = 0;

	virtual void resizeDialog() = 0;

	virtual ~EditorInterface() = default;
};

class Editor final : public EditorInterface
{
private:
	tgui::Gui& gui;
	tgui::Theme& theme;
	tgui::Canvas::Ptr& canvas;
	ResizeDialog dialog = ResizeDialog(gui);
	Camera camera = Camera(canvas);
	EditorStateManager stateMgr;
	sf::CircleShape mouseIndicator;

	bool initialized = false;
	std::string configPath;
	sf::Vector2u levelSize;

	bool isMouseWithinBoundaries(const sf::Vector2f& mousePos) const;

	bool canScroll() const
	{
		// If property window is opened, prevent scrolling
		return gui.get<tgui::ChildWindow>("ToolPropertyModal") == nullptr;
	}

	bool canOpenPropertyDialog() const
	{
		// If property window is opened, do not open new one
		return canScroll();
	}

public:
	virtual bool isInitialized() const override
	{
		return initialized;
	}

	virtual void handleEvent(const sf::Event& event, const sf::Vector2i& mousePos) override;

	virtual void draw() override;

	/**
	 *  Initialize Editor object with new level - it has some fixed width and height
	 *  Also there is path to config json which should be loaded and given to each
	 *  instantiated Tool.
	 */
	virtual void init(unsigned levelWidth, unsigned levelHeight, const std::string& configPath) override;

	virtual void switchTool(const ToolType tool) override;

	virtual void loadFromFile(const std::string& filename) override;

	virtual void saveToFile(const std::string& filename) override;

	virtual void resizeDialog() override;

	Editor(tgui::Gui& gui, tgui::Theme& theme, tgui::Canvas::Ptr& canvas, std::function<void(void)> onStateChanged);
};