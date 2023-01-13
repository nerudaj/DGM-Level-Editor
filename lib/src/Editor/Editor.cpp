#include "include/Editor/Editor.hpp"
#include "include/JsonHelper.hpp"
#include "include/LogConsole.hpp"
#include "include/Commands/ResizeCommand.hpp"
#include "include/Utilities/Utilities.hpp"

#include <fstream>
#include <filesystem>

const sf::Vector2f LEFT_VEC(-24.f, 0.f);
const sf::Vector2f UP_VEC(0.f, -24.f);
const sf::Vector2f DOWN_VEC(0.f, 24.f);
const sf::Vector2f RIGHT_VEC(24.f, 0.f);

constexpr bool Editor::isMouseWithinBoundaries(const sf::Vector2f& mousePos) const noexcept
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
	shortcutEngine->unregisterShortcutGroup(MENU_NAME);

	auto addEditorMenuItem = [&](
		const std::string& label,
		std::function<void(void)> callback,
		sf::Keyboard::Key shortcut)
	{
		menu->addMenuItem(label);
		menu->connectMenuItem(MENU_NAME, label, callback);

		shortcutEngine->registerShortcut(
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
	auto prop = stateMgr.getActiveTool()
		.getProperty(physicalPen.getCurrentPenPos());

	if (!canOpenPropertyDialog() || !prop.has_value())
		return;

	currentlyOpenedProperty = std::move(prop.value());

	currentlyOpenedProperty->buildModal(
		gui,
		theme,
		stateMgr.getActiveTool());
}

void Editor::drawTagHighlight()
{
	auto&& object = stateMgr.getActiveTool()
		.getHighlightedObject(physicalPen.getCurrentPenPos());
	if (!object.has_value() || object->tag == 0)
		return;

	std::vector<sf::Vertex> positions;
	auto computeLinesToDraw = [&object, &positions] (ToolInterface& t)
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
	if (!isInitialized()) return;

	// Update mouse position for both indicator and current tool
	auto realMousePos = camera.getWorldCoordinates(sf::Vector2f(mousePos));
	mouseIndicator.setPosition(realMousePos);
	physicalPen.updatePenPosition(sf::Vector2i(realMousePos));

	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Left && canScroll()) camera.move(LEFT_VEC);
		else if (event.key.code == sf::Keyboard::Up && canScroll()) camera.move(UP_VEC);
		else if (event.key.code == sf::Keyboard::Down && canScroll()) camera.move(DOWN_VEC);
		else if (event.key.code == sf::Keyboard::Right && canScroll()) camera.move(RIGHT_VEC);
		else if (event.key.code == sf::Keyboard::Escape) physicalPen.penCancel();
		else if (event.key.code == sf::Keyboard::Delete) physicalPen.penDelete();
	}
	else if (event.type == sf::Event::MouseWheelScrolled && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
	{
		camera.zoom(event.mouseWheelScroll.delta * -0.25f);
		std::cout << "Zoom: " << camera.getCurrentZoomLevel() << std::endl;
	}
}

void Editor::draw()
{
	if (!isInitialized()) return;

	stateMgr.forallStates([this] (ToolInterface& tool, bool active)
	{
		tool.drawTo(canvas, active ? 255 : 128);
	});

	drawTagHighlight();

	canvas->draw(mouseIndicator);
}

void Editor::init(
	unsigned levelWidth,
	unsigned levelHeight,
	const std::string& configPath)
{
	auto config = JsonHelper::loadFromFile(configPath);
	config["configFolder"] = std::filesystem::path(configPath).parent_path().string();

	stateMgr.forallStates([&config] (ToolInterface& tool)
	{
		tool.configure(config);
	});

	stateMgr.forallStates([levelWidth, levelHeight] (ToolInterface& tool)
	{
		tool.resize(levelWidth, levelHeight);
	});

	// Configure camera
	camera.init();
	camera.resetPosition();
	camera.resetZoom();

	initialized = true;

	populateMenuBar();

	// By default selecting mesh tool
	switchTool(EditorState::Mesh);

	// Configure canvas callbacks
	canvas->connect("RightMouseReleased", [this]
	{
		handleRmbClicked();
	});
	canvas->connect("MousePressed", [this]
	{
		physicalPen.penDown();
	});
	canvas->connect("MouseReleased", [this]
	{
		physicalPen.penUp();
	});
}

void Editor::switchTool(EditorState state)
{
	stateMgr.changeState(state);
	stateMgr.getActiveTool().buildSidebar();

	auto menu = gui.get<tgui::MenuBar>("TopMenuBar");
	stateMgr.getActiveTool().buildCtxMenu(menu);
}

LevelD Editor::save() const
{
	LevelD result;

	stateMgr.forallStates([&result] (const ToolInterface& tool)
	{
		tool.saveTo(result);
	});

	return result;
}

void Editor::loadFrom(
	const LevelD& lvd,
	bool skipInit)
{
	if (not skipInit)
		// Currently using this to be able to load the config
		init(1, 1, lvd.metadata.description);

	stateMgr.forallStates([&lvd] (ToolInterface& tool)
	{
		tool.loadFrom(lvd);
	});
}

void Editor::resizeDialog()
{
	dialog.open([this]
	{
		commandQueue->push<ResizeCommand>(
			*this,
			dialog.getLevelWidth(),
			dialog.getLevelHeight());
	});
}

void Editor::resize(unsigned width, unsigned height)
{
	stateMgr.forallStates([width, height] (ToolInterface& t)
	{
		t.resize(width, height);
	});
}

void Editor::shrinkToFit()
{
	/*std::optional<sf::IntRect> boundingBox;
	stateMgr.forallStates([&boundingBox] (const ToolInterface& t)
	{
		boundingBox = Utilities::unifyRects(boundingBox, t.getBoundingBox());
	});

	boundingBox.and_then([this] (sf::IntRect& box)
	{
		stateMgr.forallStates([&box] (ToolInterface& t)
			{
				t.resize(box);
			});
	});*/
}

Editor::Editor(
	tgui::Gui& guiRef,
	tgui::Theme& themeRef,
	tgui::Canvas::Ptr& canvas,
	std::function<void(void)> onStateChanged,
	GC<CommandQueue> commandQueueRef,
	GC<ShortcutEngineInterface> shortcutEngineRef)
	: gui(guiRef)
	, theme(themeRef)
	, canvas(canvas)
	, commandQueue(commandQueueRef)
	, shortcutEngine(shortcutEngineRef)
	, physicalPen([this] () -> PenUserInterface& { return stateMgr.getActiveTool(); })
{
	// Instantiate all EditorTools here
	stateMgr.addState<ToolMesh>(
		EditorState::Mesh,
		onStateChanged, shortcutEngine, gui, theme, commandQueue);

	stateMgr.addState<ToolItem>(
		EditorState::Item,
		onStateChanged, shortcutEngine, gui, theme, commandQueue);

	stateMgr.addState<ToolTrigger>(
		EditorState::Trigger,
		onStateChanged, shortcutEngine, gui, theme, commandQueue, [this] () -> sf::Vector2i { return physicalPen.getCurrentPenPos(); });

	// Bootstrapping mouse indicator
	mouseIndicator.setRadius(8.f);
	mouseIndicator.setFillColor(sf::Color::Green);
}
