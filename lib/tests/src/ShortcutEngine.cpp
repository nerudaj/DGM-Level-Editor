#include <catch.hpp>
#include "../include/NullCallback.hpp"
#include "include/Shortcuts/ShortcutEngine.hpp"

TEST_CASE("[ShortcutEngine]")
{
	ShortcutEngine engine;

	SECTION("Construction")
	{
		SECTION("Allows adding different bindings")
		{
			// No exception or assertions should happen
			engine.registerShortcut(
				"testGroup",
				{ true, false, sf::Keyboard::A },
				Null::Callback);
			engine.registerShortcut(
				"testGroup",
				{ true, true, sf::Keyboard::A },
				Null::Callback);
			engine.registerShortcut(
				"testGroup",
				{ false, false, sf::Keyboard::A },
				Null::Callback);
		}

		SECTION("Allows to register, unregister and re-register binding")
		{
			ShortcutCombo combo = { true, false, sf::Keyboard::A };
			engine.registerShortcut("testGroup", combo, Null::Callback);
			engine.unregisterShortcutGroup("testGroup");
			engine.registerShortcut("testGroup", combo, Null::Callback);
		}

		SECTION("Unregistering nonexistent group does nothing")
		{
			engine.unregisterShortcutGroup("nonexistent");
		}
	}

	SECTION("Invocation")
	{
		SECTION("Invokes a known shortcut")
		{
			bool called = false;
			engine.registerShortcut(
				"testGroup",
				{ true, false, sf::Keyboard::A }, 
				[&] { called = true; });
			
			sf::Event event;
			event.type = sf::Event::KeyPressed;
			event.key.code = sf::Keyboard::LControl;
			engine.handleEvent(event);

			event.type = sf::Event::KeyPressed;
			event.key.code = sf::Keyboard::A;
			engine.handleEvent(event);

			REQUIRE(called);
		}
	}
}
