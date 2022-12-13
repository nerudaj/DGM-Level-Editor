#include "include/Editor/Editor.hpp"
#include "include/JsonHelper.hpp"
#include "include/LogConsole.hpp"
#include <fstream>
#include <filesystem>

const sf::Vector2f LEFT_VEC(-24.f, 0.f);
const sf::Vector2f UP_VEC(0.f, -24.f);
const sf::Vector2f DOWN_VEC(0.f, 24.f);
const sf::Vector2f RIGHT_VEC(24.f, 0.f);

bool Editor::isMouseWithinBoundaries(const sf::Vector2f& mousePos) const
{
	return mousePos.x < 0.f && mousePos.y < 0.f;
}

void Editor::populateMenuBar()
{
	const std::string MENU_NAME = "Editor";
	const std::string MENU_MESH_MODE = "Mesh mode (M)";
	const std::string MENU_ITEM_MODE = "Items mode (I)";
	const std::string MENU_TRIG_MODE = "Trigger mode (T)";
	const std::string MENU_RESIZE = "Resize level (R)";
	const std::string MENU_SHRINK = "Shrink level to fit (S)";

	auto menu = gui.get<tgui::MenuBar>("TopMenuBar");

	// Cleanup previously built data
	menu->removeMenu(MENU_NAME);
	shortcutEngine.unregisterShortcutGroup(MENU_NAME);

	auto addEditorMenuItem = [&](
		const std::string& label,
		std::function<void(void)> callback,
		sf::Keyboard::Key shortcut)
	{
		menu->addMenuItem(label);
		menu->connectMenuItem(MENU_NAME, label, callback);

		shortcutEngine.registerShortcut(
			MENU_NAME,
			{ false, false, shortcut },
			callback);
	};

	// Build menu
	menu->addMenu(MENU_NAME);
	addEditorMenuItem(
		MENU_MESH_MODE,
		[this] { switchTool(EditorState::Mesh); },
		sf::Keyboard::M);
	addEditorMenuItem(
		MENU_ITEM_MODE,
		[this] { switchTool(EditorState::Item); },
		sf::Keyboard::I);
	addEditorMenuItem(
		MENU_TRIG_MODE,
		[this] { switchTool(EditorState::Trigger); },
		sf::Keyboard::T);
	addEditorMenuItem(
		MENU_RESIZE,
		[this] { resizeDialog(); },
		sf::Keyboard::R);
	addEditorMenuItem(
		MENU_SHRINK,
		[this] { shrinkToFit(); },
		sf::Keyboard::S);
}

void Editor::handleRmbClicked()
{
	currentlyOpenedProperty = stateMgr.getActiveTool().getProperty();

	if (not canOpenPropertyDialog() or !currentlyOpenedProperty)
		return;

	currentlyOpenedProperty->buildModal(
		gui,
		theme,
		stateMgr.getActiveTool());
}

void Editor::drawTagHighlight()
{
	auto&& object = stateMgr.getActiveTool().getHighlightedObject();
	if (!object.has_value() || object->tag == 0)
		return;

	std::vector<sf::Vertex> positions;
	auto computeLinesToDraw = [&object, &positions] (Tool& t)
	{
		const auto vec = t.getPositionsOfObjectsWithTag(object->tag);
		for (auto&& pos : vec)
		{
			positions.push_back(sf::Vertex(sf::Vector2f(object->position)));
			positions.push_back(sf::Vertex(sf::Vector2f(pos)));
		}
	};

	stateMgr.forallStates(computeLinesToDraw);
	canvas->draw(positions.data(), positions.size(), sf::Lines);
}

void Editor::handleEvent(const sf::Event& event, const sf::Vector2i& mousePos)
{
	if (!initialized) return;

	// Update mouse position for both indicator and current tool
	auto realMousePos = camera.getWorldCoordinates(sf::Vector2f(mousePos));
	mouseIndicator.setPosition(realMousePos);
	stateMgr.getActiveTool().penPosition(sf::Vector2i(realMousePos));

	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Left && canScroll()) camera.move(LEFT_VEC);
		else if (event.key.code == sf::Keyboard::Up && canScroll()) camera.move(UP_VEC);
		else if (event.key.code == sf::Keyboard::Down && canScroll()) camera.move(DOWN_VEC);
		else if (event.key.code == sf::Keyboard::Right && canScroll()) camera.move(RIGHT_VEC);
		else if (event.key.code == sf::Keyboard::Escape) stateMgr.getActiveTool().penCancel();
		else if (event.key.code == sf::Keyboard::Delete) stateMgr.getActiveTool().penDelete();
	}
	else if (event.type == sf::Event::MouseWheelScrolled && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
	{
		camera.zoom(event.mouseWheelScroll.delta * -0.25f);
		std::cout << "Zoom: " << camera.getCurrentZoomLevel() << std::endl;
	}
}

void Editor::draw()
{
	if (!initialized) return;

	stateMgr.forallStates([this] (Tool& tool, bool active)
	{
		tool.drawTo(canvas, active ? 255 : 128);
	});

	drawTagHighlight();

	canvas->draw(mouseIndicator);
}

void Editor::init(unsigned levelWidth, unsigned levelHeight, const std::string& configPath)
{
	levelSize = { levelWidth, levelHeight };

	// Load json config and configure all tools
	auto config = JsonHelper::loadFromFile(configPath);
	config["configFolder"] = std::filesystem::path(configPath).parent_path().string();
	stateMgr.forallStates([levelWidth, levelHeight, &config] (Tool& tool)
	{
		tool.configure(config);
	tool.resize(levelWidth, levelHeight);
	});

	// Configure camera
	camera.init();
	camera.resetPosition();
	camera.resetZoom();

	initialized = true;
	Editor::configPath = configPath; // Remember this and export to leveld later

	populateMenuBar();

	// By default selecting mesh tool
	switchTool(EditorState::Mesh);

	// Configure canvas callbacks
	canvas->connect("RightMouseReleased", [this] { handleRmbClicked(); });
	canvas->connect("MousePressed", [this] { stateMgr.getActiveTool().penDown(); });
	canvas->connect("MouseReleased", [this] { stateMgr.getActiveTool().penUp(); });
}

void Editor::switchTool(EditorState state)
{
	stateMgr.changeState(state);
	stateMgr.getActiveTool().buildSidebar(theme);

	auto menu = gui.get<tgui::MenuBar>("TopMenuBar");
	stateMgr.getActiveTool().buildCtxMenu(menu);
}

LevelD Editor::save() const
{
	LevelD result;

	stateMgr.forallStates([&result] (const Tool& tool)
	{
		tool.saveTo(result);
	});

	return result;
}

void Editor::loadFrom(const LevelD& lvd)
{
	init(1, 1, lvd.metadata.description); // Currently using this to be able to load the config
	stateMgr.forallStates([&lvd] (Tool& tool)
	{
		tool.loadFrom(lvd);
	});
}

void Editor::resizeDialog()
{
	auto handleResizeDialog = [this] ()
	{
		unsigned width = dialog.getLevelWidth();
		unsigned height = dialog.getLevelHeight();
		stateMgr.forallStates([&] (Tool& tool)
		{
			tool.resize(width, height);
		});
	};

	dialog.open(handleResizeDialog);
}

void Editor::shrinkToFit()
{}

Editor::Editor(
	tgui::Gui& gui,
	tgui::Theme& theme,
	tgui::Canvas::Ptr& canvas,
	std::function<void(void)> onStateChanged,
	CommandQueue& commandQueue,
	ShortcutEngineInterface& shortcutEngine)
	: gui(gui)
	, theme(theme)
	, canvas(canvas)
	, commandQueue(commandQueue)
	, shortcutEngine(shortcutEngine)
{
	// Instantiate all EditorTools here
	stateMgr.addState<ToolMesh>(
		EditorState::Mesh,
		gui, onStateChanged, commandQueue, shortcutEngine);
	stateMgr.addState<ToolItem>(
		EditorState::Item,
		gui, onStateChanged, commandQueue, shortcutEngine);
	stateMgr.addState<ToolTrigger>(
		EditorState::Trigger,
		gui, onStateChanged, commandQueue, shortcutEngine);

	// Bootstrapping mouse indicator
	mouseIndicator.setRadius(8.f);
	mouseIndicator.setFillColor(sf::Color::Green);
}
