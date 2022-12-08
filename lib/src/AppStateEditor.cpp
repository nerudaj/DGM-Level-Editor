#include <cmath>

#include "include/AppStateEditor.hpp"
#include "include/Dialogs/NewLevelDialog.hpp"
#include "include/Configs/Sizers.hpp"
#include "include/Globals.hpp"
#include <iostream>

void AppStateEditor::handleExit(YesNoCancelDialogInterface& confirmExitDialog)
{
	if (unsavedChanges)
	{
		confirmExitDialog.open(DIALOG_TITLE_WARNING, DIALOG_TEXT_UNSAVED_EXIT, [&] (UserChoice choice)
			{
				if (choice == UserChoice::Cancelled)
				return;
				else if (choice == UserChoice::Confirmed)
					saveLevel();
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

std::optional<std::string> AppStateEditor::getNewSavePath()
{
	try
	{
		auto result = fileApi->getSaveFileName("LevelD Files\0*.lvd\0Any File\0*.*\0");

		if (not result.ends_with(".lvd"))
			return result + ".lvd";
		return result;
	}
	catch (...)
	{
		return std::nullopt;
	}
}

void AppStateEditor::handleUndo()
{
	Log::write("Undo");
}

void AppStateEditor::handleRedo()
{
	Log::write("Redo");
}

void AppStateEditor::handleShortcut(const sf::Keyboard::Key modKey, const sf::Keyboard::Key mainKey)
{
	if (modKey == sf::Keyboard::LControl && appShortcuts.contains(mainKey))
	{
		appShortcuts.at(mainKey)();
	}
	else if (modKey == sf::Keyboard::LShift && editorShortcuts.contains(mainKey) && editor->isInitialized())
	{
		editorShortcuts.at(mainKey)();
	}
	else
	{
		Log::write(std::format("Unknown shortcut {} + {}", std::to_string(modKey), std::to_string(mainKey)));
	}
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
			handleExit(dialogConfirmExit);
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
			if (event.key.code == sf::Keyboard::LControl || event.key.code == sf::Keyboard::LShift)
			{
				lastPressedModKey = event.key.code;
			}
			else if (event.key.code == sf::Keyboard::Enter)
			{
				if (dialogNewLevel.isOpen())
				{
					newLevelDialogCallback();
					dialogNewLevel.close();
				}
			}
			else
			{
				handleShortcut(lastPressedModKey, event.key.code);
			}
		}
		else if (event.type == sf::Event::KeyReleased)
		{
			if (event.key.code == sf::Keyboard::LControl || event.key.code == sf::Keyboard::LShift)
			{
				lastPressedModKey = sf::Keyboard::Unknown;
			}
		}

		gui.handleEvent(event);
		editor->handleEvent(event, mousePos);
	}
}

void AppStateEditor::update()
{
	commandQueue.processAll();
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
	cfg::Ini& ini,
	const std::string& rootDir,
	std::unique_ptr<FileApiInterface> fileApi)
	:
	dgm::AppState(app),
	ini(ini),
	rootDir(rootDir),
	fileApi(std::move(fileApi))
{
	try
	{
		resmgr.loadResource<sf::Font>(rootDir + "/resources/cruft.ttf");
	}
	catch (std::exception& e)
	{
		std::cerr << "error:AppStateMainMenu: " << e.what() << std::endl;
		throw;
	}

	// Setup resources
	sf::Font& font = resmgr.get<sf::Font>("cruft.ttf");

	editor = std::make_unique<Editor>(gui, theme, canvas, [&] ()
	{
		unsavedChanges = true;
	updateWindowTitle();
	}, commandQueue);

	// Gui setup
	theme.load(rootDir + "/resources/TransparentGrey.txt");

	gui.setFont(font);
	gui.setTarget(app.window.getWindowContext());

	buildLayout();

	updateWindowTitle();
}

constexpr const char* FILE_CTX_NEW = "New (Ctrl+N)";
constexpr const char* FILE_CTX_LOAD = "Load (Ctrl+O)";
constexpr const char* FILE_CTX_SAVE = "Save (Ctrl+S)";
constexpr const char* FILE_CTX_SAVE_AS = "Save as...";
constexpr const char* FILE_CTX_UNDO = "Undo (Ctrl+Z)";
constexpr const char* FILE_CTX_REDO = "Redo (Ctrl+Y)";
constexpr const char* FILE_CTX_EXIT = "Exit";

void AppStateEditor::buildLayout()
{
	const std::string TOPBAR_HEIGHT = std::to_string(Sizers::GetMenuBarHeight());
	const std::string SIDEBAR_WIDTH = "8%";
	const std::string SIDEBAR_HEIGHT = "&.height - " + TOPBAR_HEIGHT;
	const std::string SIDEBAR_XPOS = "&.width - " + SIDEBAR_WIDTH;

	// Canvas
	canvas = tgui::Canvas::create();
	// NOTE: If canvas is getting some "shadow" tile edits out of the view (under sidebar)
	// it might be because sidebar doesn't stop events from propagating downwards
	canvas->setSize(app.window.getSize().x, SIDEBAR_HEIGHT);
	canvas->setPosition(0.f, TOPBAR_HEIGHT);
	gui.add(canvas, "TilesetCanvas");

	// Top bar
	auto menu = tgui::MenuBar::create();
	menu->setTextSize(Sizers::GetMenuBarTextHeight());
	menu->setRenderer(theme.getRenderer("MenuBar"));
	menu->getRenderer()->setTextColor(sf::Color::Black);

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
			assert(!appShortcuts.contains(shortcut.value()));
			appShortcuts[shortcut.value()] = callback;
		}
	};

	addFileMenuItem(FILE_CTX_NEW, [this] { newLevelDialogCallback(); }, sf::Keyboard::N);
	addFileMenuItem(FILE_CTX_LOAD, [this] { loadLevel(); }, sf::Keyboard::O);
	addFileMenuItem(FILE_CTX_SAVE, [this] { saveLevel(); }, sf::Keyboard::S);
	addFileMenuItem(FILE_CTX_SAVE_AS, [this] { saveLevel(true); });
	addFileMenuItem(FILE_CTX_UNDO, [this] { handleUndo(); }, sf::Keyboard::Z);
	addFileMenuItem(FILE_CTX_REDO, [this] { handleRedo(); }, sf::Keyboard::Y);
	addFileMenuItem(FILE_CTX_EXIT, [this] { app.popState(); });

	menu->addMenu("View");
	menu->addMenuItem("Console");
	menu->connectMenuItem("View", "Console", [this] () { Log::get().toggle(); });

	menu->addMenu("Editor");
	auto addEditorMenuItem = [this, &menu](
		const std::string& label,
		std::function<void(void)> callback,
		sf::Keyboard::Key shortcut)
	{
		menu->addMenuItem(label);
		menu->connectMenuItem("Editor", label, callback);
		assert(!editorShortcuts.contains(shortcut));
		editorShortcuts[shortcut] = callback;
	};

	addEditorMenuItem(
		"Mesh mode (M)",
		[this] { editor->switchTool(Editor::ToolType::Mesh); },
		sf::Keyboard::M);
	addEditorMenuItem("Items mode (I)",
		[this] { editor->switchTool(Editor::ToolType::Item); },
		sf::Keyboard::I);
	addEditorMenuItem("Trigger mode (T)",
		[this] { editor->switchTool(Editor::ToolType::Trigger); },
		sf::Keyboard::T);
	addEditorMenuItem("Resize level (R)",
		[this] { editor->resizeDialog(); },
		sf::Keyboard::R);
	addEditorMenuItem("Shrink level to fit (S)",
		[this] { editor->shrinkToFit(); },
		sf::Keyboard::S);

	// Must be added AFTER canvas, otherwise canvas blocks pop-up menus
	gui.add(menu, "TopMenuBar");

	// Side bar - only bootstrap the space it will be sitting in
	auto sidebar = tgui::Group::create();
	sidebar->setSize(SIDEBAR_WIDTH, SIDEBAR_HEIGHT);
	sidebar->setPosition(SIDEBAR_XPOS, TOPBAR_HEIGHT);
	gui.add(sidebar, "Sidebar");

	// Logger console
	Log::get().init(&gui);
	Log::get().create(theme, { "0.5%", "81%" }, { "20%", "15%" });
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

void AppStateEditor::loadLevel()
{
	Log::write("AppStateEditor::loadLevel(). savePath = " + savePath + "; empty = " + std::to_string(savePath.empty()));

	try
	{
		savePath = fileApi->getOpenFileName("LevelD Files\0*.lvd\0Any File\0*.*\0");
	}
	catch (...) { return; } // User cancel


	// The load path becomes save path for subsequent saves
	try
	{
		filePath = savePath;
		unsavedChanges = false;
		editor->loadFromFile(savePath);
		updateWindowTitle();
	}
	catch (std::exception& e)
	{
		Log::write(e.what());
	}
}

void AppStateEditor::saveLevel(bool forceNewPath)
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
		editor->saveToFile(savePath);
		updateWindowTitle();
	}
	catch (std::exception& e)
	{
		Log::write(e.what());
	}
}

