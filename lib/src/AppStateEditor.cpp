#include <cmath>

#include "include/AppStateEditor.hpp"
#include "include/Dialogs/NewLevelDialog.hpp"
#include "include/Configs/Sizers.hpp"
#include "include/Globals.hpp"
#include "include/Editor/Editor.hpp"
#include "include/Utilities/FontLoader.hpp"

#include <iostream>

constexpr const char* FILE_CTX_NEW = "New (Ctrl+N)";
constexpr const char* FILE_CTX_LOAD = "Load (Ctrl+O)";
constexpr const char* FILE_CTX_SAVE = "Save (Ctrl+S)";
constexpr const char* FILE_CTX_SAVE_AS = "Save as...";
constexpr const char* FILE_CTX_UNDO = "Undo (Ctrl+Z)";
constexpr const char* FILE_CTX_REDO = "Redo (Ctrl+Y)";
constexpr const char* FILE_CTX_EXIT = "Exit";

void AppStateEditor::handleExit(YesNoCancelDialogInterface& confirmExitDialog)
{
	if (unsavedChanges)
	{
		confirmExitDialog.open(DIALOG_TITLE_WARNING, DIALOG_TEXT_UNSAVED_EXIT, [&] (UserChoice choice)
			{
				if (choice == UserChoice::Cancelled)
				return;
				else if (choice == UserChoice::Confirmed)
					handleSaveLevel();
				else
					unsavedChanges = false;
		handleExit(confirmExitDialog);
			});
	}
	else
	{
		app.exit();
	}
}

void AppStateEditor::setupFont()
{
	FontLoader fontLoader(resmgr);
	bool fontLoaded = fontLoader.loadFonts({
		"C:/WINDOWS/FONTS/SEGOEUI.TTF",
		rootDir + "/resources/cruft.ttf"
	});

	if (!fontLoaded)
	{
		throw std::runtime_error("Could not load any font.");
	}

	gui.setFont(
		resmgr.get<sf::Font>(fontLoader.getLoadedFontName()));
}

std::optional<std::string> AppStateEditor::getNewSavePath()
{
	auto result = fileApi->getSaveFileName("LevelD Files\0*.lvd\0Any File\0*.*\0");
	return result.transform([] (const std::string& s) -> std::string
	{
		return s.ends_with(".lvd") ? s : s + ".lvd";
	});
}

void AppStateEditor::input()
{
	const sf::Vector2i mousePos = sf::Mouse::getPosition(
		app.window.getWindowContext());

	sf::Event event;
	while (app.window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			handleExit(*dialogConfirmExit);
		}
		else if (event.type == sf::Event::Resized)
		{
			app.window.getWindowContext().setView(
				sf::View(
					sf::FloatRect(
						0.f, 0.f,
						static_cast<float>(event.size.width),
						static_cast<float>(event.size.height))));
			gui.removeAllWidgets();
			gui.setView(app.window.getWindowContext().getView());
			buildLayout();
		}
		else if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Enter)
			{
				if (dialogNewLevel.isOpen())
				{
					newLevelDialogCallback();
					dialogNewLevel.close();
				}
			}
		}
		else if (event.type == sf::Event::GainedFocus)
		{
			sf::Event releaseKeys;
			releaseKeys.type = sf::Event::KeyReleased;
			releaseKeys.key.code = sf::Keyboard::LShift;
			shortcutEngine->handleEvent(releaseKeys);
			releaseKeys.key.code = sf::Keyboard::LControl;
			shortcutEngine->handleEvent(releaseKeys);
		}

		gui.handleEvent(event);
		editor->handleEvent(event, mousePos);
		shortcutEngine->handleEvent(event);
	}
}

void AppStateEditor::update()
{
	if (commandQueue->isEmpty())
		return;

	unsavedChanges = true;
	updateWindowTitle();

	commandQueue->processAll();
}

void AppStateEditor::draw()
{
	// Drawing canvas
	canvas->clear();

	editor->draw();

	canvas->display();

	// Whole window
	gui.draw();
}

AppStateEditor::AppStateEditor(
	dgm::App& app,
	tgui::Gui& gui,
	tgui::Theme& theme,
	cfg::Ini& ini,
	const std::string& rootDir,
	GC<FileApiInterface> fileApi,
	GC<ShortcutEngineInterface> shortcutEngine,
	GC<YesNoCancelDialogInterface> dialogConfirmExit,
	GC<ErrorInfoDialogInterface> dialogErrorInfo)
	: dgm::AppState(app)
	, gui(gui)
	, theme(theme)
	, ini(ini)
	, rootDir(rootDir)
	, fileApi(fileApi)
	, shortcutEngine(shortcutEngine)
	, dialogConfirmExit(dialogConfirmExit)
	, dialogErrorInfo(dialogErrorInfo)
{
	try
	{
		theme.load(rootDir + "/resources/TransparentGrey.txt");
		setupFont();
	}
	catch (std::exception& e)
	{
		std::cerr << "error:AppStateMainMenu: " << e.what() << std::endl;
		throw;
	}

	// Setup resources
	gui.setTarget(app.window.getWindowContext());

	buildLayout();

	auto onStateChanged = [this]
	{
		unsavedChanges = true;
		updateWindowTitle();
	};

	editor = Box<Editor>(
		gui,
		theme,
		canvas,
		onStateChanged,
		commandQueue,
		this->shortcutEngine);

	updateWindowTitle();
}

void AppStateEditor::buildLayout()
{
	const std::string TOPBAR_HEIGHT = std::to_string(Sizers::GetMenuBarHeight());
	const unsigned TOPBAR_FONT_HEIGHT = Sizers::GetMenuBarTextHeight();
	const std::string SIDEBAR_WIDTH = "8%";
	// 2* because of the logger
	const std::string SIDEBAR_HEIGHT = "&.height - 2*" + TOPBAR_HEIGHT;
	const std::string SIDEBAR_XPOS = "&.width - " + SIDEBAR_WIDTH;

	// Canvas
	auto runToken = buildCanvasLayout(
		SIDEBAR_WIDTH,
		SIDEBAR_HEIGHT,
		TOPBAR_HEIGHT);
	buildMenuBarLayout(
		runToken,
		TOPBAR_HEIGHT,
		TOPBAR_FONT_HEIGHT);
	buildSidebarLayout(
		runToken,
		SIDEBAR_WIDTH,
		SIDEBAR_HEIGHT,
		TOPBAR_HEIGHT);
	auto loggerChatBox = buildLoggerLayout(
		runToken,
		TOPBAR_HEIGHT,
		TOPBAR_FONT_HEIGHT);
	Log::get().init(loggerChatBox);
}

AppStateEditor::AllowExecutionToken AppStateEditor::buildCanvasLayout(
	const std::string& SIDEBAR_WIDTH,
	const std::string& SIDEBAR_HEIGHT,
	const std::string& TOPBAR_HEIGHT)
{
	canvas = tgui::Canvas::create();
	canvas->setSize("100% - " + SIDEBAR_WIDTH, SIDEBAR_HEIGHT);
	canvas->setPosition(0.f, TOPBAR_HEIGHT);
	gui.add(canvas, "TilesetCanvas");
	return AllowExecutionToken();
}

void AppStateEditor::buildMenuBarLayout(
	AllowExecutionToken,
	const std::string& TOPBAR_HEIGHT,
	unsigned TOPBAR_FONT_HEIGHT)
{
	auto menu = tgui::MenuBar::create();
	menu->setTextSize(TOPBAR_FONT_HEIGHT);
	menu->setRenderer(theme.getRenderer("MenuBar"));
	menu->setSize("100%", TOPBAR_HEIGHT);
	menu->addMenu("File");

	auto addFileMenuItem = [this, &menu](
		const std::string& label,
		std::function<void(void)> callback,
		std::optional<sf::Keyboard::Key> shortcut = {})
	{
		menu->addMenuItem(label);
		menu->connectMenuItem("File", label, callback);

		if (shortcut.has_value())
		{
			shortcutEngine->registerShortcut(
				"FileShortcuts",
				{ true, false, *shortcut },
				callback);
		}
	};

	addFileMenuItem(FILE_CTX_NEW, [this] { handleNewLevel(); }, sf::Keyboard::N);
	addFileMenuItem(FILE_CTX_LOAD, [this] { handleLoadLevel(); }, sf::Keyboard::O);
	addFileMenuItem(FILE_CTX_SAVE, [this] { handleSaveLevel(); }, sf::Keyboard::S);
	addFileMenuItem(FILE_CTX_SAVE_AS, [this] { handleSaveLevel(true); });
	addFileMenuItem(FILE_CTX_UNDO, [this] { handleUndo(); }, sf::Keyboard::Z);
	addFileMenuItem(FILE_CTX_REDO, [this] { handleRedo(); }, sf::Keyboard::Y);
	addFileMenuItem(FILE_CTX_EXIT, [this] { handleExit(*dialogConfirmExit); });

	gui.add(menu, "TopMenuBar");
}

void AppStateEditor::buildSidebarLayout(
	AllowExecutionToken,
	const std::string& SIDEBAR_WIDTH,
	const std::string& SIDEBAR_HEIGHT,
	const std::string& TOPBAR_HEIGHT)
{
	// only bootstrap the space it will be sitting in
	auto sidebar = tgui::Group::create();
	sidebar->setSize(SIDEBAR_WIDTH, SIDEBAR_HEIGHT);
	sidebar->setPosition("100% - " + SIDEBAR_WIDTH, TOPBAR_HEIGHT);
	gui.add(sidebar, "Sidebar");
}

tgui::ChatBox::Ptr AppStateEditor::buildLoggerLayout(
	AllowExecutionToken,
	const std::string& TOPBAR_HEIGHT,
	unsigned TOPBAR_FONT_HEIGHT)
{
	auto logger = tgui::ChatBox::create();
	logger->setRenderer(theme.getRenderer("ChatBox"));
	logger->setSize("100%", TOPBAR_HEIGHT);
	logger->setPosition("0%", "100% - " + TOPBAR_HEIGHT);
	logger->setTextSize(Sizers::GetMenuBarTextHeight());
	logger->setLinesStartFromTop();
	logger->setLineLimit(1);
	logger->addLine("This is a log console");
	gui.add(logger, "LoggerBox");
	return logger;
}

void AppStateEditor::newLevelDialogCallback()
{
	savePath = "";
	updateWindowTitle();

	// Get settings from modal
	unsigned levelWidth = dialogNewLevel.getLevelWidth();
	unsigned levelHeight = dialogNewLevel.getLevelHeight();
	std::string configPath = dialogNewLevel.getConfigPath();

	Log::write("editor.init(" + std::to_string(levelWidth) + ", " + std::to_string(levelHeight) + ", " + configPath + ");");
	editor->init(levelWidth, levelHeight, configPath);
	ini["Editor"]["configPath"] = configPath;
}

void AppStateEditor::handleNewLevel()
{
	dialogNewLevel.open([this] { newLevelDialogCallback(); });
}

void AppStateEditor::handleLoadLevel()
{
	auto r = fileApi->getOpenFileName("LevelD Files\0*.lvd\0Any File\0*.*\0");
	if (!r.has_value()) return;

	savePath = *r;
	filePath = savePath; // The load path becomes save path for subsequent saves

	try
	{
		LevelD lvd;
		lvd.loadFromFile(savePath);
		configPath = lvd.metadata.description;

		if (configPath.empty())
			throw std::runtime_error("Path to config file is unknown!");

		editor->loadFrom(lvd);
		unsavedChanges = false;
		updateWindowTitle();
		Log::write2("Level loaded from '{}'", savePath);
	}
	catch (std::exception& e)
	{
		dialogErrorInfo->open(e.what());
	}
}

void AppStateEditor::handleSaveLevel(bool forceNewPath) noexcept
{
	if (savePath.empty() || forceNewPath)
	{
		if (auto str = getNewSavePath())
			savePath = *str;
		else // user cancelled
			return;
	}

	try
	{
		filePath = savePath;
		unsavedChanges = false;
		auto&& lvd = editor->save();
		lvd.metadata.description = configPath;
		lvd.saveToFile(savePath);
		updateWindowTitle();
		Log::write2("Level saved to '{}'", savePath);
	}
	catch (std::exception& e)
	{
		dialogErrorInfo->open(e.what());
	}
}

void AppStateEditor::handleUndo()
{
	Log::write("Undo");
	commandHistory->undo();
}

void AppStateEditor::handleRedo()
{
	Log::write("Redo");
	commandHistory->redo();
}
