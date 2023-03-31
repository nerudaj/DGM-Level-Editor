#include <catch.hpp>

#include <include/Editor/Editor.hpp>

#include "TestHelpers/NullCallback.hpp"
#include "TestHelpers/TestAssets.hpp"
#include "TestHelpers/LeveldBuilder.hpp"

TEST_CASE("[Editor]")
{
	tgui::Gui gui;
	gui.add(tgui::MenuBar::create(), "TopMenuBar");
	gui.add(tgui::Group::create(), "Sidebar");

	tgui::Theme theme;
	sf::View view;
	tgui::Canvas::Ptr canvas = tgui::Canvas::create();
	canvas->setView(view);
	CommandHistory commandHistory;
	GC<CommandQueue> commandQueue(commandHistory);
	GC<ShortcutEngine> shortcutEngine;

	Editor editor(
		gui,
		theme,
		canvas,
		Null::Callback,
		commandQueue,
		shortcutEngine);

	SECTION("loadFrom")
	{
		SECTION("Can load another level when one was already loaded")
		{
			LevelD level1 = LeveldBuilder::buildWithMesh(32, 20, {});
			LevelD level2 = LeveldBuilder::buildWithMesh(40, 40, {});

			editor.loadFrom(level1, Config::CONFIG_PATH);

			// FIXME: Throws exception
			editor.loadFrom(level2, Config::CONFIG_PATH);
		}
	}
}
